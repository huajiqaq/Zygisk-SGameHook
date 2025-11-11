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

#ifndef ZYGISK_SGAMEHOOK_HOOK_H
#define ZYGISK_SGAMEHOOK_HOOK_H

void il2cpp_api_init(void *handle);

void il2cpp_hook(const char *outDir);

#endif //ZYGISK_SGAMEHOOK_HOOK_H
