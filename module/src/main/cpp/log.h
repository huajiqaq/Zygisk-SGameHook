/*
 * SGameHook - Android Zygisk Module
 * Modified by: huajiqaq
 * Original work by: Perfare (2020/7/4)
 *
 * This project is based on Il2CppDumper and other open source projects.
 * See LICENSE file for full license information.
 */

// Created by Perfare on 2020/7/4.

#ifndef ZYGISK_SGAMEHOOK_LOG_H
#define ZYGISK_SGAMEHOOK_LOG_H

#include <android/log.h>

#define LOG_TAG "ZYGISK_SGAMEHOOK"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

#endif //ZYGISK_SGAMEHOOK_LOG_H