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

#ifndef ZYGISK_SGAMEHOOK_HACK_H
#define ZYGISK_SGAMEHOOK_HACK_H

#include <stddef.h>

void hack_prepare(const char *game_data_dir, void *data, size_t length);

#endif //ZYGISK_SGAMEHOOK_HACK_H
