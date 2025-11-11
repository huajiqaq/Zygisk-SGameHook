/*
 * SGameHook - Android Zygisk Module
 * Modified by: huajiqaq
 * Original work by: Perfare (2020/7/4)
 *
 * This project is based on Il2CppDumper and other open source projects.
 * See LICENSE file for full license information.
 */

//
// Created by Perfare on 2020/7/4.
//

#include "il2cpp_hook.h"
#include <dlfcn.h>
#include <cstdlib>
#include <cstring>
#include <cinttypes>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include "xdl.h"
#include "log.h"
#include "il2cpp-class.h"
#include "dobby.h"

#define DO_API(r, n, p) r (*n) p

#include "il2cpp-api-functions.h"

#undef DO_API

static uint64_t il2cpp_base = 0;

void init_il2cpp_api(void *handle) {
#define DO_API(r, n, p) {                      \
    n = (r (*) p)xdl_sym(handle, #n, nullptr); \
    if(!n) {                                   \
        LOGW("api not found %s", #n);          \
    }                                          \
}

#include "il2cpp-api-functions.h"

#undef DO_API
}

void* get_method_ptr(const char* asmName, const char* ns, const char* cls, const char* method, int paramCount) {
    auto domain = il2cpp_domain_get();
    auto assembly = il2cpp_domain_assembly_open(domain, asmName);
    if (!assembly) {
        LOGE("Failed to get assembly: %s", asmName);
        return nullptr;
    }
    auto image = il2cpp_assembly_get_image(assembly);
    if (!image) {
        LOGE("Failed to get image from assembly: %s", asmName);
        return nullptr;
    }
    auto klass = il2cpp_class_from_name(image, ns, cls);
    if (!klass) {
        LOGE("Failed to get class: %s.%s", ns, cls);
        return nullptr;
    }

    auto methodInfo = il2cpp_class_get_method_from_name(klass, method, paramCount);
    if (!methodInfo) {
        LOGE("Failed to get method: %s.%s::%s with %d parameters", ns, cls, method, paramCount);
        return nullptr;
    }

    LOGI("Successfully resolved method pointer: %s.%s::%s -> %p", ns, cls, method, methodInfo->methodPointer);
    return (void*)methodInfo->methodPointer;
}

void il2cpp_api_init(void *handle) {
    LOGI("il2cpp_handle: %p", handle);
    init_il2cpp_api(handle);
    if (il2cpp_domain_get_assemblies) {
        Dl_info dlInfo;
        if (dladdr((void *) il2cpp_domain_get_assemblies, &dlInfo)) {
            il2cpp_base = reinterpret_cast<uint64_t>(dlInfo.dli_fbase);
        }
        LOGI("il2cpp_base: %" PRIx64"", il2cpp_base);
    } else {
        LOGE("Failed to initialize il2cpp api.");
        return;
    }
    while (!il2cpp_is_vm_thread(nullptr)) {
        LOGI("Waiting for il2cpp_init...");
        sleep(1);
    }
    auto domain = il2cpp_domain_get();
    il2cpp_thread_attach(domain);
}

void* create_il2cpp_byte_array(const uint8_t* data, size_t size) {
    auto domain = il2cpp_domain_get();
    auto assembly = il2cpp_domain_assembly_open(domain, "mscorlib");
    auto image = il2cpp_assembly_get_image(assembly);
    auto byteClass = il2cpp_class_from_name(image, "System", "Byte");
    auto arr = il2cpp_array_new(byteClass, size);
    if (!arr) return nullptr;
    memcpy(reinterpret_cast<uint8_t*>(arr) + sizeof(void*) * 4, data, size);
    return arr;
}

const char* CUSTOM_TEX_JPG_FILE = "customtexjpg";
const char* CUSTOM_TEX_PNG_FILE = "customtexpng";
const char* get_custom_texture_filename(bool is_png) {
    return is_png ? CUSTOM_TEX_PNG_FILE : CUSTOM_TEX_JPG_FILE;
}

const char* CURRENT_GAMEPATH = nullptr;
void* read_custom_texture_file(const char* filename) {
    char filepath[512];
    snprintf(filepath, sizeof(filepath), "%s/files/%s", CURRENT_GAMEPATH, filename);
    LOGI("[+] Reading custom texture file: %s", filepath);

    FILE* fp = fopen(filepath, "rb");
    if (!fp) {
        LOGE("[-] Failed to open: %s", filepath);
        return nullptr;
    }

    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    rewind(fp);
    if (size == 0) {
        fclose(fp);
        LOGE("[-] File is empty: %s", filepath);
        return nullptr;
    }

    uint8_t* buffer = static_cast<uint8_t*>(malloc(size));
    if (!buffer) {
        fclose(fp);
        LOGE("[-] malloc failed (size=%zu)", size);
        return nullptr;
    }

    fread(buffer, 1, size, fp);
    fclose(fp);

    void* arr = create_il2cpp_byte_array(buffer, size);
    free(buffer);
    if (!arr) {
        LOGE("[-] create_il2cpp_byte_array failed");
    }
    return arr;
}

bool (*orig_LoadImage)(void* texture, void* data, bool markNonReadable) = nullptr;
void* (*orig_EncodeTexture2DToJPG)(void* tex, int quality) = nullptr;
void* (*orig_TextureEncodeToPNG)(void* tex) = nullptr;

void* hook_EncodeTexture2DToJPG(void* tex, int quality) {
    void* custom = read_custom_texture_file(get_custom_texture_filename(false));
    if (custom && orig_LoadImage) {
        LOGI("[+] Replacing JPG texture");
        orig_LoadImage(tex, custom, false);
    }
    return orig_EncodeTexture2DToJPG(tex, quality);
}

void* hook_TextureEncodeToPNG(void* tex) {
    void* custom = read_custom_texture_file(get_custom_texture_filename(true));
    if (custom && orig_LoadImage) {
        LOGI("[+] Replacing PNG texture");
        orig_LoadImage(tex, custom, false);
    }
    return orig_TextureEncodeToPNG(tex);
}

void il2cpp_hook(const char *outDir) {
    LOGI("hooking...");
    CURRENT_GAMEPATH = outDir;
    orig_LoadImage = (bool (*)(void*, void*, bool))get_method_ptr("UnityEngine.ImageConversionModule", "UnityEngine", "ImageConversion", "LoadImage", 3);
    void* encodeTexturePtr = get_method_ptr("Scripts.Base", "Assets.Scripts.UI", "CUIUtility", "EncodeTexture2DToJPG", 2);
    if (encodeTexturePtr) {
        DobbyHook(encodeTexturePtr,
                  (void *)hook_EncodeTexture2DToJPG,
                  (void **)&orig_EncodeTexture2DToJPG);
        LOGI("[+] Hooked EncodeTexture2DToJPG");
    }

    if (strstr(outDir, "com.tencent.tmgp.sgamece")) {
        void* textureEncodePtr = get_method_ptr("Scripts.Base", "Assets.Scripts.UI", "CUIUtility", "TextureEncodeToPNG", 1);
        if (textureEncodePtr) {
            DobbyHook(textureEncodePtr,
                      (void *)hook_TextureEncodeToPNG,
                      (void **)&orig_TextureEncodeToPNG);
            LOGI("[+] Hooked TextureEncodeToPNG (sgamece)");
        }
    }
    LOGI("hook done!");
}