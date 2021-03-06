/*
 * Copyright 2012 Dmitry Timoshkov
 * Copyright 2012 Hans Leidekker for CodeWeavers
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#include <stdarg.h>
#include <stdio.h>

#define COBJMACROS

#include "windef.h"
#include "wincodec.h"
#include "wine/test.h"

/* 1x1 pixel PNG image */
static const char png_no_color_profile[] = {
  0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d,
  0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01,
  0x08, 0x02, 0x00, 0x00, 0x00, 0x90, 0x77, 0x53, 0xde, 0x00, 0x00, 0x00,
  0x09, 0x70, 0x48, 0x59, 0x73, 0x00, 0x00, 0x0b, 0x13, 0x00, 0x00, 0x0b,
  0x13, 0x01, 0x00, 0x9a, 0x9c, 0x18, 0x00, 0x00, 0x00, 0x07, 0x74, 0x49,
  0x4d, 0x45, 0x07, 0xdc, 0x0b, 0x0e, 0x0e, 0x22, 0x17, 0x10, 0xd8, 0xde,
  0x3b, 0x00, 0x00, 0x00, 0x0c, 0x49, 0x44, 0x41, 0x54, 0x08, 0xd7, 0x63,
  0xf8, 0xff, 0xff, 0x3f, 0x00, 0x05, 0xfe, 0x02, 0xfe, 0xdc, 0xcc, 0x59,
  0xe7, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4e, 0x44, 0xae, 0x42, 0x60,
  0x82
};

/* 1x1 pixel PNG image with embedded sRGB profile */
static const char png_color_profile[] = {
  0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d,
  0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01,
  0x08, 0x02, 0x00, 0x00, 0x00, 0x90, 0x77, 0x53, 0xde, 0x00, 0x00, 0x0a,
  0x43, 0x69, 0x43, 0x43, 0x50, 0x49, 0x43, 0x43, 0x20, 0x70, 0x72, 0x6f,
  0x66, 0x69, 0x6c, 0x65, 0x00, 0x00, 0x78, 0xda, 0x9d, 0x53, 0x77, 0x58,
  0x93, 0xf7, 0x16, 0x3e, 0xdf, 0xf7, 0x65, 0x0f, 0x56, 0x42, 0xd8, 0xf0,
  0xb1, 0x97, 0x6c, 0x81, 0x00, 0x22, 0x23, 0xac, 0x08, 0xc8, 0x10, 0x59,
  0xa2, 0x10, 0x92, 0x00, 0x61, 0x84, 0x10, 0x12, 0x40, 0xc5, 0x85, 0x88,
  0x0a, 0x56, 0x14, 0x15, 0x11, 0x9c, 0x48, 0x55, 0xc4, 0x82, 0xd5, 0x0a,
  0x48, 0x9d, 0x88, 0xe2, 0xa0, 0x28, 0xb8, 0x67, 0x41, 0x8a, 0x88, 0x5a,
  0x8b, 0x55, 0x5c, 0x38, 0xee, 0x1f, 0xdc, 0xa7, 0xb5, 0x7d, 0x7a, 0xef,
  0xed, 0xed, 0xfb, 0xd7, 0xfb, 0xbc, 0xe7, 0x9c, 0xe7, 0xfc, 0xce, 0x79,
  0xcf, 0x0f, 0x80, 0x11, 0x12, 0x26, 0x91, 0xe6, 0xa2, 0x6a, 0x00, 0x39,
  0x52, 0x85, 0x3c, 0x3a, 0xd8, 0x1f, 0x8f, 0x4f, 0x48, 0xc4, 0xc9, 0xbd,
  0x80, 0x02, 0x15, 0x48, 0xe0, 0x04, 0x20, 0x10, 0xe6, 0xcb, 0xc2, 0x67,
  0x05, 0xc5, 0x00, 0x00, 0xf0, 0x03, 0x79, 0x78, 0x7e, 0x74, 0xb0, 0x3f,
  0xfc, 0x01, 0xaf, 0x6f, 0x00, 0x02, 0x00, 0x70, 0xd5, 0x2e, 0x24, 0x12,
  0xc7, 0xe1, 0xff, 0x83, 0xba, 0x50, 0x26, 0x57, 0x00, 0x20, 0x91, 0x00,
  0xe0, 0x22, 0x12, 0xe7, 0x0b, 0x01, 0x90, 0x52, 0x00, 0xc8, 0x2e, 0x54,
  0xc8, 0x14, 0x00, 0xc8, 0x18, 0x00, 0xb0, 0x53, 0xb3, 0x64, 0x0a, 0x00,
  0x94, 0x00, 0x00, 0x6c, 0x79, 0x7c, 0x42, 0x22, 0x00, 0xaa, 0x0d, 0x00,
  0xec, 0xf4, 0x49, 0x3e, 0x05, 0x00, 0xd8, 0xa9, 0x93, 0xdc, 0x17, 0x00,
  0xd8, 0xa2, 0x1c, 0xa9, 0x08, 0x00, 0x8d, 0x01, 0x00, 0x99, 0x28, 0x47,
  0x24, 0x02, 0x40, 0xbb, 0x00, 0x60, 0x55, 0x81, 0x52, 0x2c, 0x02, 0xc0,
  0xc2, 0x00, 0xa0, 0xac, 0x40, 0x22, 0x2e, 0x04, 0xc0, 0xae, 0x01, 0x80,
  0x59, 0xb6, 0x32, 0x47, 0x02, 0x80, 0xbd, 0x05, 0x00, 0x76, 0x8e, 0x58,
  0x90, 0x0f, 0x40, 0x60, 0x00, 0x80, 0x99, 0x42, 0x2c, 0xcc, 0x00, 0x20,
  0x38, 0x02, 0x00, 0x43, 0x1e, 0x13, 0xcd, 0x03, 0x20, 0x4c, 0x03, 0xa0,
  0x30, 0xd2, 0xbf, 0xe0, 0xa9, 0x5f, 0x70, 0x85, 0xb8, 0x48, 0x01, 0x00,
  0xc0, 0xcb, 0x95, 0xcd, 0x97, 0x4b, 0xd2, 0x33, 0x14, 0xb8, 0x95, 0xd0,
  0x1a, 0x77, 0xf2, 0xf0, 0xe0, 0xe2, 0x21, 0xe2, 0xc2, 0x6c, 0xb1, 0x42,
  0x61, 0x17, 0x29, 0x10, 0x66, 0x09, 0xe4, 0x22, 0x9c, 0x97, 0x9b, 0x23,
  0x13, 0x48, 0xe7, 0x03, 0x4c, 0xce, 0x0c, 0x00, 0x00, 0x1a, 0xf9, 0xd1,
  0xc1, 0xfe, 0x38, 0x3f, 0x90, 0xe7, 0xe6, 0xe4, 0xe1, 0xe6, 0x66, 0xe7,
  0x6c, 0xef, 0xf4, 0xc5, 0xa2, 0xfe, 0x6b, 0xf0, 0x6f, 0x22, 0x3e, 0x21,
  0xf1, 0xdf, 0xfe, 0xbc, 0x8c, 0x02, 0x04, 0x00, 0x10, 0x4e, 0xcf, 0xef,
  0xda, 0x5f, 0xe5, 0xe5, 0xd6, 0x03, 0x70, 0xc7, 0x01, 0xb0, 0x75, 0xbf,
  0x6b, 0xa9, 0x5b, 0x00, 0xda, 0x56, 0x00, 0x68, 0xdf, 0xf9, 0x5d, 0x33,
  0xdb, 0x09, 0xa0, 0x5a, 0x0a, 0xd0, 0x7a, 0xf9, 0x8b, 0x79, 0x38, 0xfc,
  0x40, 0x1e, 0x9e, 0xa1, 0x50, 0xc8, 0x3c, 0x1d, 0x1c, 0x0a, 0x0b, 0x0b,
  0xed, 0x25, 0x62, 0xa1, 0xbd, 0x30, 0xe3, 0x8b, 0x3e, 0xff, 0x33, 0xe1,
  0x6f, 0xe0, 0x8b, 0x7e, 0xf6, 0xfc, 0x40, 0x1e, 0xfe, 0xdb, 0x7a, 0xf0,
  0x00, 0x71, 0x9a, 0x40, 0x99, 0xad, 0xc0, 0xa3, 0x83, 0xfd, 0x71, 0x61,
  0x6e, 0x76, 0xae, 0x52, 0x8e, 0xe7, 0xcb, 0x04, 0x42, 0x31, 0x6e, 0xf7,
  0xe7, 0x23, 0xfe, 0xc7, 0x85, 0x7f, 0xfd, 0x8e, 0x29, 0xd1, 0xe2, 0x34,
  0xb1, 0x5c, 0x2c, 0x15, 0x8a, 0xf1, 0x58, 0x89, 0xb8, 0x50, 0x22, 0x4d,
  0xc7, 0x79, 0xb9, 0x52, 0x91, 0x44, 0x21, 0xc9, 0x95, 0xe2, 0x12, 0xe9,
  0x7f, 0x32, 0xf1, 0x1f, 0x96, 0xfd, 0x09, 0x93, 0x77, 0x0d, 0x00, 0xac,
  0x86, 0x4f, 0xc0, 0x4e, 0xb6, 0x07, 0xb5, 0xcb, 0x6c, 0xc0, 0x7e, 0xee,
  0x01, 0x02, 0x8b, 0x0e, 0x58, 0xd2, 0x76, 0x00, 0x40, 0x7e, 0xf3, 0x2d,
  0x8c, 0x1a, 0x0b, 0x91, 0x00, 0x10, 0x67, 0x34, 0x32, 0x79, 0xf7, 0x00,
  0x00, 0x93, 0xbf, 0xf9, 0x8f, 0x40, 0x2b, 0x01, 0x00, 0xcd, 0x97, 0xa4,
  0xe3, 0x00, 0x00, 0xbc, 0xe8, 0x18, 0x5c, 0xa8, 0x94, 0x17, 0x4c, 0xc6,
  0x08, 0x00, 0x00, 0x44, 0xa0, 0x81, 0x2a, 0xb0, 0x41, 0x07, 0x0c, 0xc1,
  0x14, 0xac, 0xc0, 0x0e, 0x9c, 0xc1, 0x1d, 0xbc, 0xc0, 0x17, 0x02, 0x61,
  0x06, 0x44, 0x40, 0x0c, 0x24, 0xc0, 0x3c, 0x10, 0x42, 0x06, 0xe4, 0x80,
  0x1c, 0x0a, 0xa1, 0x18, 0x96, 0x41, 0x19, 0x54, 0xc0, 0x3a, 0xd8, 0x04,
  0xb5, 0xb0, 0x03, 0x1a, 0xa0, 0x11, 0x9a, 0xe1, 0x10, 0xb4, 0xc1, 0x31,
  0x38, 0x0d, 0xe7, 0xe0, 0x12, 0x5c, 0x81, 0xeb, 0x70, 0x17, 0x06, 0x60,
  0x18, 0x9e, 0xc2, 0x18, 0xbc, 0x86, 0x09, 0x04, 0x41, 0xc8, 0x08, 0x13,
  0x61, 0x21, 0x3a, 0x88, 0x11, 0x62, 0x8e, 0xd8, 0x22, 0xce, 0x08, 0x17,
  0x99, 0x8e, 0x04, 0x22, 0x61, 0x48, 0x34, 0x92, 0x80, 0xa4, 0x20, 0xe9,
  0x88, 0x14, 0x51, 0x22, 0xc5, 0xc8, 0x72, 0xa4, 0x02, 0xa9, 0x42, 0x6a,
  0x91, 0x5d, 0x48, 0x23, 0xf2, 0x2d, 0x72, 0x14, 0x39, 0x8d, 0x5c, 0x40,
  0xfa, 0x90, 0xdb, 0xc8, 0x20, 0x32, 0x8a, 0xfc, 0x8a, 0xbc, 0x47, 0x31,
  0x94, 0x81, 0xb2, 0x51, 0x03, 0xd4, 0x02, 0x75, 0x40, 0xb9, 0xa8, 0x1f,
  0x1a, 0x8a, 0xc6, 0xa0, 0x73, 0xd1, 0x74, 0x34, 0x0f, 0x5d, 0x80, 0x96,
  0xa2, 0x6b, 0xd1, 0x1a, 0xb4, 0x1e, 0x3d, 0x80, 0xb6, 0xa2, 0xa7, 0xd1,
  0x4b, 0xe8, 0x75, 0x74, 0x00, 0x7d, 0x8a, 0x8e, 0x63, 0x80, 0xd1, 0x31,
  0x0e, 0x66, 0x8c, 0xd9, 0x61, 0x5c, 0x8c, 0x87, 0x45, 0x60, 0x89, 0x58,
  0x1a, 0x26, 0xc7, 0x16, 0x63, 0xe5, 0x58, 0x35, 0x56, 0x8f, 0x35, 0x63,
  0x1d, 0x58, 0x37, 0x76, 0x15, 0x1b, 0xc0, 0x9e, 0x61, 0xef, 0x08, 0x24,
  0x02, 0x8b, 0x80, 0x13, 0xec, 0x08, 0x5e, 0x84, 0x10, 0xc2, 0x6c, 0x82,
  0x90, 0x90, 0x47, 0x58, 0x4c, 0x58, 0x43, 0xa8, 0x25, 0xec, 0x23, 0xb4,
  0x12, 0xba, 0x08, 0x57, 0x09, 0x83, 0x84, 0x31, 0xc2, 0x27, 0x22, 0x93,
  0xa8, 0x4f, 0xb4, 0x25, 0x7a, 0x12, 0xf9, 0xc4, 0x78, 0x62, 0x3a, 0xb1,
  0x90, 0x58, 0x46, 0xac, 0x26, 0xee, 0x21, 0x1e, 0x21, 0x9e, 0x25, 0x5e,
  0x27, 0x0e, 0x13, 0x5f, 0x93, 0x48, 0x24, 0x0e, 0xc9, 0x92, 0xe4, 0x4e,
  0x0a, 0x21, 0x25, 0x90, 0x32, 0x49, 0x0b, 0x49, 0x6b, 0x48, 0xdb, 0x48,
  0x2d, 0xa4, 0x53, 0xa4, 0x3e, 0xd2, 0x10, 0x69, 0x9c, 0x4c, 0x26, 0xeb,
  0x90, 0x6d, 0xc9, 0xde, 0xe4, 0x08, 0xb2, 0x80, 0xac, 0x20, 0x97, 0x91,
  0xb7, 0x90, 0x0f, 0x90, 0x4f, 0x92, 0xfb, 0xc9, 0xc3, 0xe4, 0xb7, 0x14,
  0x3a, 0xc5, 0x88, 0xe2, 0x4c, 0x09, 0xa2, 0x24, 0x52, 0xa4, 0x94, 0x12,
  0x4a, 0x35, 0x65, 0x3f, 0xe5, 0x04, 0xa5, 0x9f, 0x32, 0x42, 0x99, 0xa0,
  0xaa, 0x51, 0xcd, 0xa9, 0x9e, 0xd4, 0x08, 0xaa, 0x88, 0x3a, 0x9f, 0x5a,
  0x49, 0x6d, 0xa0, 0x76, 0x50, 0x2f, 0x53, 0x87, 0xa9, 0x13, 0x34, 0x75,
  0x9a, 0x25, 0xcd, 0x9b, 0x16, 0x43, 0xcb, 0xa4, 0x2d, 0xa3, 0xd5, 0xd0,
  0x9a, 0x69, 0x67, 0x69, 0xf7, 0x68, 0x2f, 0xe9, 0x74, 0xba, 0x09, 0xdd,
  0x83, 0x1e, 0x45, 0x97, 0xd0, 0x97, 0xd2, 0x6b, 0xe8, 0x07, 0xe9, 0xe7,
  0xe9, 0x83, 0xf4, 0x77, 0x0c, 0x0d, 0x86, 0x0d, 0x83, 0xc7, 0x48, 0x62,
  0x28, 0x19, 0x6b, 0x19, 0x7b, 0x19, 0xa7, 0x18, 0xb7, 0x19, 0x2f, 0x99,
  0x4c, 0xa6, 0x05, 0xd3, 0x97, 0x99, 0xc8, 0x54, 0x30, 0xd7, 0x32, 0x1b,
  0x99, 0x67, 0x98, 0x0f, 0x98, 0x6f, 0x55, 0x58, 0x2a, 0xf6, 0x2a, 0x7c,
  0x15, 0x91, 0xca, 0x12, 0x95, 0x3a, 0x95, 0x56, 0x95, 0x7e, 0x95, 0xe7,
  0xaa, 0x54, 0x55, 0x73, 0x55, 0x3f, 0xd5, 0x79, 0xaa, 0x0b, 0x54, 0xab,
  0x55, 0x0f, 0xab, 0x5e, 0x56, 0x7d, 0xa6, 0x46, 0x55, 0xb3, 0x50, 0xe3,
  0xa9, 0x09, 0xd4, 0x16, 0xab, 0xd5, 0xa9, 0x1d, 0x55, 0xbb, 0xa9, 0x36,
  0xae, 0xce, 0x52, 0x77, 0x52, 0x8f, 0x50, 0xcf, 0x51, 0x5f, 0xa3, 0xbe,
  0x5f, 0xfd, 0x82, 0xfa, 0x63, 0x0d, 0xb2, 0x86, 0x85, 0x46, 0xa0, 0x86,
  0x48, 0xa3, 0x54, 0x63, 0xb7, 0xc6, 0x19, 0x8d, 0x21, 0x16, 0xc6, 0x32,
  0x65, 0xf1, 0x58, 0x42, 0xd6, 0x72, 0x56, 0x03, 0xeb, 0x2c, 0x6b, 0x98,
  0x4d, 0x62, 0x5b, 0xb2, 0xf9, 0xec, 0x4c, 0x76, 0x05, 0xfb, 0x1b, 0x76,
  0x2f, 0x7b, 0x4c, 0x53, 0x43, 0x73, 0xaa, 0x66, 0xac, 0x66, 0x91, 0x66,
  0x9d, 0xe6, 0x71, 0xcd, 0x01, 0x0e, 0xc6, 0xb1, 0xe0, 0xf0, 0x39, 0xd9,
  0x9c, 0x4a, 0xce, 0x21, 0xce, 0x0d, 0xce, 0x7b, 0x2d, 0x03, 0x2d, 0x3f,
  0x2d, 0xb1, 0xd6, 0x6a, 0xad, 0x66, 0xad, 0x7e, 0xad, 0x37, 0xda, 0x7a,
  0xda, 0xbe, 0xda, 0x62, 0xed, 0x72, 0xed, 0x16, 0xed, 0xeb, 0xda, 0xef,
  0x75, 0x70, 0x9d, 0x40, 0x9d, 0x2c, 0x9d, 0xf5, 0x3a, 0x6d, 0x3a, 0xf7,
  0x75, 0x09, 0xba, 0x36, 0xba, 0x51, 0xba, 0x85, 0xba, 0xdb, 0x75, 0xcf,
  0xea, 0x3e, 0xd3, 0x63, 0xeb, 0x79, 0xe9, 0x09, 0xf5, 0xca, 0xf5, 0x0e,
  0xe9, 0xdd, 0xd1, 0x47, 0xf5, 0x6d, 0xf4, 0xa3, 0xf5, 0x17, 0xea, 0xef,
  0xd6, 0xef, 0xd1, 0x1f, 0x37, 0x30, 0x34, 0x08, 0x36, 0x90, 0x19, 0x6c,
  0x31, 0x38, 0x63, 0xf0, 0xcc, 0x90, 0x63, 0xe8, 0x6b, 0x98, 0x69, 0xb8,
  0xd1, 0xf0, 0x84, 0xe1, 0xa8, 0x11, 0xcb, 0x68, 0xba, 0x91, 0xc4, 0x68,
  0xa3, 0xd1, 0x49, 0xa3, 0x27, 0xb8, 0x26, 0xee, 0x87, 0x67, 0xe3, 0x35,
  0x78, 0x17, 0x3e, 0x66, 0xac, 0x6f, 0x1c, 0x62, 0xac, 0x34, 0xde, 0x65,
  0xdc, 0x6b, 0x3c, 0x61, 0x62, 0x69, 0x32, 0xdb, 0xa4, 0xc4, 0xa4, 0xc5,
  0xe4, 0xbe, 0x29, 0xcd, 0x94, 0x6b, 0x9a, 0x66, 0xba, 0xd1, 0xb4, 0xd3,
  0x74, 0xcc, 0xcc, 0xc8, 0x2c, 0xdc, 0xac, 0xd8, 0xac, 0xc9, 0xec, 0x8e,
  0x39, 0xd5, 0x9c, 0x6b, 0x9e, 0x61, 0xbe, 0xd9, 0xbc, 0xdb, 0xfc, 0x8d,
  0x85, 0xa5, 0x45, 0x9c, 0xc5, 0x4a, 0x8b, 0x36, 0x8b, 0xc7, 0x96, 0xda,
  0x96, 0x7c, 0xcb, 0x05, 0x96, 0x4d, 0x96, 0xf7, 0xac, 0x98, 0x56, 0x3e,
  0x56, 0x79, 0x56, 0xf5, 0x56, 0xd7, 0xac, 0x49, 0xd6, 0x5c, 0xeb, 0x2c,
  0xeb, 0x6d, 0xd6, 0x57, 0x6c, 0x50, 0x1b, 0x57, 0x9b, 0x0c, 0x9b, 0x3a,
  0x9b, 0xcb, 0xb6, 0xa8, 0xad, 0x9b, 0xad, 0xc4, 0x76, 0x9b, 0x6d, 0xdf,
  0x14, 0xe2, 0x14, 0x8f, 0x29, 0xd2, 0x29, 0xf5, 0x53, 0x6e, 0xda, 0x31,
  0xec, 0xfc, 0xec, 0x0a, 0xec, 0x9a, 0xec, 0x06, 0xed, 0x39, 0xf6, 0x61,
  0xf6, 0x25, 0xf6, 0x6d, 0xf6, 0xcf, 0x1d, 0xcc, 0x1c, 0x12, 0x1d, 0xd6,
  0x3b, 0x74, 0x3b, 0x7c, 0x72, 0x74, 0x75, 0xcc, 0x76, 0x6c, 0x70, 0xbc,
  0xeb, 0xa4, 0xe1, 0x34, 0xc3, 0xa9, 0xc4, 0xa9, 0xc3, 0xe9, 0x57, 0x67,
  0x1b, 0x67, 0xa1, 0x73, 0x9d, 0xf3, 0x35, 0x17, 0xa6, 0x4b, 0x90, 0xcb,
  0x12, 0x97, 0x76, 0x97, 0x17, 0x53, 0x6d, 0xa7, 0x8a, 0xa7, 0x6e, 0x9f,
  0x7a, 0xcb, 0x95, 0xe5, 0x1a, 0xee, 0xba, 0xd2, 0xb5, 0xd3, 0xf5, 0xa3,
  0x9b, 0xbb, 0x9b, 0xdc, 0xad, 0xd9, 0x6d, 0xd4, 0xdd, 0xcc, 0x3d, 0xc5,
  0x7d, 0xab, 0xfb, 0x4d, 0x2e, 0x9b, 0x1b, 0xc9, 0x5d, 0xc3, 0x3d, 0xef,
  0x41, 0xf4, 0xf0, 0xf7, 0x58, 0xe2, 0x71, 0xcc, 0xe3, 0x9d, 0xa7, 0x9b,
  0xa7, 0xc2, 0xf3, 0x90, 0xe7, 0x2f, 0x5e, 0x76, 0x5e, 0x59, 0x5e, 0xfb,
  0xbd, 0x1e, 0x4f, 0xb3, 0x9c, 0x26, 0x9e, 0xd6, 0x30, 0x6d, 0xc8, 0xdb,
  0xc4, 0x5b, 0xe0, 0xbd, 0xcb, 0x7b, 0x60, 0x3a, 0x3e, 0x3d, 0x65, 0xfa,
  0xce, 0xe9, 0x03, 0x3e, 0xc6, 0x3e, 0x02, 0x9f, 0x7a, 0x9f, 0x87, 0xbe,
  0xa6, 0xbe, 0x22, 0xdf, 0x3d, 0xbe, 0x23, 0x7e, 0xd6, 0x7e, 0x99, 0x7e,
  0x07, 0xfc, 0x9e, 0xfb, 0x3b, 0xfa, 0xcb, 0xfd, 0x8f, 0xf8, 0xbf, 0xe1,
  0x79, 0xf2, 0x16, 0xf1, 0x4e, 0x05, 0x60, 0x01, 0xc1, 0x01, 0xe5, 0x01,
  0xbd, 0x81, 0x1a, 0x81, 0xb3, 0x03, 0x6b, 0x03, 0x1f, 0x04, 0x99, 0x04,
  0xa5, 0x07, 0x35, 0x05, 0x8d, 0x05, 0xbb, 0x06, 0x2f, 0x0c, 0x3e, 0x15,
  0x42, 0x0c, 0x09, 0x0d, 0x59, 0x1f, 0x72, 0x93, 0x6f, 0xc0, 0x17, 0xf2,
  0x1b, 0xf9, 0x63, 0x33, 0xdc, 0x67, 0x2c, 0x9a, 0xd1, 0x15, 0xca, 0x08,
  0x9d, 0x15, 0x5a, 0x1b, 0xfa, 0x30, 0xcc, 0x26, 0x4c, 0x1e, 0xd6, 0x11,
  0x8e, 0x86, 0xcf, 0x08, 0xdf, 0x10, 0x7e, 0x6f, 0xa6, 0xf9, 0x4c, 0xe9,
  0xcc, 0xb6, 0x08, 0x88, 0xe0, 0x47, 0x6c, 0x88, 0xb8, 0x1f, 0x69, 0x19,
  0x99, 0x17, 0xf9, 0x7d, 0x14, 0x29, 0x2a, 0x32, 0xaa, 0x2e, 0xea, 0x51,
  0xb4, 0x53, 0x74, 0x71, 0x74, 0xf7, 0x2c, 0xd6, 0xac, 0xe4, 0x59, 0xfb,
  0x67, 0xbd, 0x8e, 0xf1, 0x8f, 0xa9, 0x8c, 0xb9, 0x3b, 0xdb, 0x6a, 0xb6,
  0x72, 0x76, 0x67, 0xac, 0x6a, 0x6c, 0x52, 0x6c, 0x63, 0xec, 0x9b, 0xb8,
  0x80, 0xb8, 0xaa, 0xb8, 0x81, 0x78, 0x87, 0xf8, 0x45, 0xf1, 0x97, 0x12,
  0x74, 0x13, 0x24, 0x09, 0xed, 0x89, 0xe4, 0xc4, 0xd8, 0xc4, 0x3d, 0x89,
  0xe3, 0x73, 0x02, 0xe7, 0x6c, 0x9a, 0x33, 0x9c, 0xe4, 0x9a, 0x54, 0x96,
  0x74, 0x63, 0xae, 0xe5, 0xdc, 0xa2, 0xb9, 0x17, 0xe6, 0xe9, 0xce, 0xcb,
  0x9e, 0x77, 0x3c, 0x59, 0x35, 0x59, 0x90, 0x7c, 0x38, 0x85, 0x98, 0x12,
  0x97, 0xb2, 0x3f, 0xe5, 0x83, 0x20, 0x42, 0x50, 0x2f, 0x18, 0x4f, 0xe5,
  0xa7, 0x6e, 0x4d, 0x1d, 0x13, 0xf2, 0x84, 0x9b, 0x85, 0x4f, 0x45, 0xbe,
  0xa2, 0x8d, 0xa2, 0x51, 0xb1, 0xb7, 0xb8, 0x4a, 0x3c, 0x92, 0xe6, 0x9d,
  0x56, 0x95, 0xf6, 0x38, 0xdd, 0x3b, 0x7d, 0x43, 0xfa, 0x68, 0x86, 0x4f,
  0x46, 0x75, 0xc6, 0x33, 0x09, 0x4f, 0x52, 0x2b, 0x79, 0x91, 0x19, 0x92,
  0xb9, 0x23, 0xf3, 0x4d, 0x56, 0x44, 0xd6, 0xde, 0xac, 0xcf, 0xd9, 0x71,
  0xd9, 0x2d, 0x39, 0x94, 0x9c, 0x94, 0x9c, 0xa3, 0x52, 0x0d, 0x69, 0x96,
  0xb4, 0x2b, 0xd7, 0x30, 0xb7, 0x28, 0xb7, 0x4f, 0x66, 0x2b, 0x2b, 0x93,
  0x0d, 0xe4, 0x79, 0xe6, 0x6d, 0xca, 0x1b, 0x93, 0x87, 0xca, 0xf7, 0xe4,
  0x23, 0xf9, 0x73, 0xf3, 0xdb, 0x15, 0x6c, 0x85, 0x4c, 0xd1, 0xa3, 0xb4,
  0x52, 0xae, 0x50, 0x0e, 0x16, 0x4c, 0x2f, 0xa8, 0x2b, 0x78, 0x5b, 0x18,
  0x5b, 0x78, 0xb8, 0x48, 0xbd, 0x48, 0x5a, 0xd4, 0x33, 0xdf, 0x66, 0xfe,
  0xea, 0xf9, 0x23, 0x0b, 0x82, 0x16, 0x7c, 0xbd, 0x90, 0xb0, 0x50, 0xb8,
  0xb0, 0xb3, 0xd8, 0xb8, 0x78, 0x59, 0xf1, 0xe0, 0x22, 0xbf, 0x45, 0xbb,
  0x16, 0x23, 0x8b, 0x53, 0x17, 0x77, 0x2e, 0x31, 0x5d, 0x52, 0xba, 0x64,
  0x78, 0x69, 0xf0, 0xd2, 0x7d, 0xcb, 0x68, 0xcb, 0xb2, 0x96, 0xfd, 0x50,
  0xe2, 0x58, 0x52, 0x55, 0xf2, 0x6a, 0x79, 0xdc, 0xf2, 0x8e, 0x52, 0x83,
  0xd2, 0xa5, 0xa5, 0x43, 0x2b, 0x82, 0x57, 0x34, 0x95, 0xa9, 0x94, 0xc9,
  0xcb, 0x6e, 0xae, 0xf4, 0x5a, 0xb9, 0x63, 0x15, 0x61, 0x95, 0x64, 0x55,
  0xef, 0x6a, 0x97, 0xd5, 0x5b, 0x56, 0x7f, 0x2a, 0x17, 0x95, 0x5f, 0xac,
  0x70, 0xac, 0xa8, 0xae, 0xf8, 0xb0, 0x46, 0xb8, 0xe6, 0xe2, 0x57, 0x4e,
  0x5f, 0xd5, 0x7c, 0xf5, 0x79, 0x6d, 0xda, 0xda, 0xde, 0x4a, 0xb7, 0xca,
  0xed, 0xeb, 0x48, 0xeb, 0xa4, 0xeb, 0x6e, 0xac, 0xf7, 0x59, 0xbf, 0xaf,
  0x4a, 0xbd, 0x6a, 0x41, 0xd5, 0xd0, 0x86, 0xf0, 0x0d, 0xad, 0x1b, 0xf1,
  0x8d, 0xe5, 0x1b, 0x5f, 0x6d, 0x4a, 0xde, 0x74, 0xa1, 0x7a, 0x6a, 0xf5,
  0x8e, 0xcd, 0xb4, 0xcd, 0xca, 0xcd, 0x03, 0x35, 0x61, 0x35, 0xed, 0x5b,
  0xcc, 0xb6, 0xac, 0xdb, 0xf2, 0xa1, 0x36, 0xa3, 0xf6, 0x7a, 0x9d, 0x7f,
  0x5d, 0xcb, 0x56, 0xfd, 0xad, 0xab, 0xb7, 0xbe, 0xd9, 0x26, 0xda, 0xd6,
  0xbf, 0xdd, 0x77, 0x7b, 0xf3, 0x0e, 0x83, 0x1d, 0x15, 0x3b, 0xde, 0xef,
  0x94, 0xec, 0xbc, 0xb5, 0x2b, 0x78, 0x57, 0x6b, 0xbd, 0x45, 0x7d, 0xf5,
  0x6e, 0xd2, 0xee, 0x82, 0xdd, 0x8f, 0x1a, 0x62, 0x1b, 0xba, 0xbf, 0xe6,
  0x7e, 0xdd, 0xb8, 0x47, 0x77, 0x4f, 0xc5, 0x9e, 0x8f, 0x7b, 0xa5, 0x7b,
  0x07, 0xf6, 0x45, 0xef, 0xeb, 0x6a, 0x74, 0x6f, 0x6c, 0xdc, 0xaf, 0xbf,
  0xbf, 0xb2, 0x09, 0x6d, 0x52, 0x36, 0x8d, 0x1e, 0x48, 0x3a, 0x70, 0xe5,
  0x9b, 0x80, 0x6f, 0xda, 0x9b, 0xed, 0x9a, 0x77, 0xb5, 0x70, 0x5a, 0x2a,
  0x0e, 0xc2, 0x41, 0xe5, 0xc1, 0x27, 0xdf, 0xa6, 0x7c, 0x7b, 0xe3, 0x50,
  0xe8, 0xa1, 0xce, 0xc3, 0xdc, 0xc3, 0xcd, 0xdf, 0x99, 0x7f, 0xb7, 0xf5,
  0x08, 0xeb, 0x48, 0x79, 0x2b, 0xd2, 0x3a, 0xbf, 0x75, 0xac, 0x2d, 0xa3,
  0x6d, 0xa0, 0x3d, 0xa1, 0xbd, 0xef, 0xe8, 0x8c, 0xa3, 0x9d, 0x1d, 0x5e,
  0x1d, 0x47, 0xbe, 0xb7, 0xff, 0x7e, 0xef, 0x31, 0xe3, 0x63, 0x75, 0xc7,
  0x35, 0x8f, 0x57, 0x9e, 0xa0, 0x9d, 0x28, 0x3d, 0xf1, 0xf9, 0xe4, 0x82,
  0x93, 0xe3, 0xa7, 0x64, 0xa7, 0x9e, 0x9d, 0x4e, 0x3f, 0x3d, 0xd4, 0x99,
  0xdc, 0x79, 0xf7, 0x4c, 0xfc, 0x99, 0x6b, 0x5d, 0x51, 0x5d, 0xbd, 0x67,
  0x43, 0xcf, 0x9e, 0x3f, 0x17, 0x74, 0xee, 0x4c, 0xb7, 0x5f, 0xf7, 0xc9,
  0xf3, 0xde, 0xe7, 0x8f, 0x5d, 0xf0, 0xbc, 0x70, 0xf4, 0x22, 0xf7, 0x62,
  0xdb, 0x25, 0xb7, 0x4b, 0xad, 0x3d, 0xae, 0x3d, 0x47, 0x7e, 0x70, 0xfd,
  0xe1, 0x48, 0xaf, 0x5b, 0x6f, 0xeb, 0x65, 0xf7, 0xcb, 0xed, 0x57, 0x3c,
  0xae, 0x74, 0xf4, 0x4d, 0xeb, 0x3b, 0xd1, 0xef, 0xd3, 0x7f, 0xfa, 0x6a,
  0xc0, 0xd5, 0x73, 0xd7, 0xf8, 0xd7, 0x2e, 0x5d, 0x9f, 0x79, 0xbd, 0xef,
  0xc6, 0xec, 0x1b, 0xb7, 0x6e, 0x26, 0xdd, 0x1c, 0xb8, 0x25, 0xba, 0xf5,
  0xf8, 0x76, 0xf6, 0xed, 0x17, 0x77, 0x0a, 0xee, 0x4c, 0xdc, 0x5d, 0x7a,
  0x8f, 0x78, 0xaf, 0xfc, 0xbe, 0xda, 0xfd, 0xea, 0x07, 0xfa, 0x0f, 0xea,
  0x7f, 0xb4, 0xfe, 0xb1, 0x65, 0xc0, 0x6d, 0xe0, 0xf8, 0x60, 0xc0, 0x60,
  0xcf, 0xc3, 0x59, 0x0f, 0xef, 0x0e, 0x09, 0x87, 0x9e, 0xfe, 0x94, 0xff,
  0xd3, 0x87, 0xe1, 0xd2, 0x47, 0xcc, 0x47, 0xd5, 0x23, 0x46, 0x23, 0x8d,
  0x8f, 0x9d, 0x1f, 0x1f, 0x1b, 0x0d, 0x1a, 0xbd, 0xf2, 0x64, 0xce, 0x93,
  0xe1, 0xa7, 0xb2, 0xa7, 0x13, 0xcf, 0xca, 0x7e, 0x56, 0xff, 0x79, 0xeb,
  0x73, 0xab, 0xe7, 0xdf, 0xfd, 0xe2, 0xfb, 0x4b, 0xcf, 0x58, 0xfc, 0xd8,
  0xf0, 0x0b, 0xf9, 0x8b, 0xcf, 0xbf, 0xae, 0x79, 0xa9, 0xf3, 0x72, 0xef,
  0xab, 0xa9, 0xaf, 0x3a, 0xc7, 0x23, 0xc7, 0x1f, 0xbc, 0xce, 0x79, 0x3d,
  0xf1, 0xa6, 0xfc, 0xad, 0xce, 0xdb, 0x7d, 0xef, 0xb8, 0xef, 0xba, 0xdf,
  0xc7, 0xbd, 0x1f, 0x99, 0x28, 0xfc, 0x40, 0xfe, 0x50, 0xf3, 0xd1, 0xfa,
  0x63, 0xc7, 0xa7, 0xd0, 0x4f, 0xf7, 0x3e, 0xe7, 0x7c, 0xfe, 0xfc, 0x2f,
  0xf7, 0x84, 0xf3, 0xfb, 0x80, 0x39, 0x25, 0x11, 0x00, 0x00, 0x00, 0x09,
  0x70, 0x48, 0x59, 0x73, 0x00, 0x00, 0x0b, 0x13, 0x00, 0x00, 0x0b, 0x13,
  0x01, 0x00, 0x9a, 0x9c, 0x18, 0x00, 0x00, 0x00, 0x07, 0x74, 0x49, 0x4d,
  0x45, 0x07, 0xdc, 0x0b, 0x0e, 0x0e, 0x25, 0x16, 0x28, 0x9e, 0x78, 0x6a,
  0x00, 0x00, 0x00, 0x0c, 0x49, 0x44, 0x41, 0x54, 0x08, 0xd7, 0x63, 0xf8,
  0xff, 0xff, 0x3f, 0x00, 0x05, 0xfe, 0x02, 0xfe, 0xdc, 0xcc, 0x59, 0xe7,
  0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82
};

static IWICImagingFactory *factory;

static IWICBitmapDecoder *create_decoder(const void *image_data, UINT image_size)
{
    HGLOBAL hmem;
    BYTE *data;
    HRESULT hr;
    IWICBitmapDecoder *decoder = NULL;
    IStream *stream;
    GUID format;

    hmem = GlobalAlloc(0, image_size);
    data = GlobalLock(hmem);
    memcpy(data, image_data, image_size);
    GlobalUnlock(hmem);

    hr = CreateStreamOnHGlobal(hmem, TRUE, &stream);
    ok(hr == S_OK, "CreateStreamOnHGlobal error %#x\n", hr);

    hr = IWICImagingFactory_CreateDecoderFromStream(factory, stream, NULL, 0, &decoder);
    ok(hr == S_OK, "CreateDecoderFromStream error %#x\n", hr);

    hr = IWICBitmapDecoder_GetContainerFormat(decoder, &format);
    ok(hr == S_OK, "GetContainerFormat error %#x\n", hr);
    ok(IsEqualGUID(&format, &GUID_ContainerFormatPng),
       "wrong container format %s\n", wine_dbgstr_guid(&format));

    IStream_Release(stream);

    return decoder;
}

static WCHAR *save_profile( BYTE *buffer, UINT size )
{
    static const WCHAR tstW[] = {'t','s','t',0};
    WCHAR path[MAX_PATH], filename[MAX_PATH], *ret;
    HANDLE handle;
    DWORD count;

    GetTempPathW(MAX_PATH, path);
    GetTempFileNameW(path, tstW, 0, filename);

    handle = CreateFileW(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
    if (handle == INVALID_HANDLE_VALUE) return NULL;

    WriteFile(handle, buffer, size, &count, NULL);
    CloseHandle( handle );

    ret = HeapAlloc(GetProcessHeap(), 0, (lstrlenW(filename) + 1) * sizeof(WCHAR));
    lstrcpyW(ret, filename);
    return ret;
}

static void test_color_contexts(void)
{
    HRESULT hr;
    IWICBitmapDecoder *decoder;
    IWICBitmapFrameDecode *frame;
    IWICColorContext *context;
    WICColorContextType type;
    UINT count, colorspace, size;
    WCHAR *tmpfile;
    BYTE *buffer;
    BOOL ret;

    decoder = create_decoder(png_no_color_profile, sizeof(png_no_color_profile));
    ok(decoder != 0, "Failed to load PNG image data\n");

    /* global color context */
    hr = IWICBitmapDecoder_GetColorContexts(decoder, 0, NULL, NULL);
    ok(hr == WINCODEC_ERR_UNSUPPORTEDOPERATION, "GetColorContexts error %#x\n", hr);

    count = 0xdeadbeef;
    hr = IWICBitmapDecoder_GetColorContexts(decoder, 0, NULL, &count);
    ok(hr == WINCODEC_ERR_UNSUPPORTEDOPERATION, "GetColorContexts error %#x\n", hr);
    ok(count == 0xdeadbeef, "unexpected count %u\n", count);

    /* frame color context */
    hr = IWICBitmapDecoder_GetFrame(decoder, 0, &frame);
    ok(hr == S_OK, "GetFrame error %#x\n", hr);

    hr = IWICBitmapFrameDecode_GetColorContexts(frame, 0, NULL, NULL);
    ok(hr == E_INVALIDARG, "expected E_INVALIDARG, got %#x\n", hr);

    count = 0xdeadbeef;
    hr = IWICBitmapFrameDecode_GetColorContexts(frame, 0, NULL, &count);
    ok(hr == S_OK, "GetColorContexts error %#x\n", hr);
    ok(!count, "unexpected count %u\n", count);

    IWICBitmapFrameDecode_Release(frame);
    IWICBitmapDecoder_Release(decoder);

    decoder = create_decoder(png_color_profile, sizeof(png_color_profile));
    ok(decoder != 0, "Failed to load PNG image data\n");

    /* global color context */
    count = 0xdeadbeef;
    hr = IWICBitmapDecoder_GetColorContexts(decoder, 0, NULL, &count);
    ok(hr == WINCODEC_ERR_UNSUPPORTEDOPERATION, "GetColorContexts error %#x\n", hr);
    ok(count == 0xdeadbeef, "unexpected count %u\n", count);

    /* frame color context */
    hr = IWICBitmapDecoder_GetFrame(decoder, 0, &frame);
    ok(hr == S_OK, "GetFrame error %#x\n", hr);

    count = 0xdeadbeef;
    hr = IWICBitmapFrameDecode_GetColorContexts(frame, 0, NULL, &count);
    ok(hr == S_OK, "GetColorContexts error %#x\n", hr);
    ok(count == 1, "unexpected count %u\n", count);

    hr = IWICImagingFactory_CreateColorContext(factory, NULL);
    ok(hr == E_INVALIDARG, "expected E_INVALIDARG, got %#x\n", hr);

    hr = IWICImagingFactory_CreateColorContext(factory, &context);
    ok(hr == S_OK, "CreateColorContext error %#x\n", hr);

    hr = IWICColorContext_GetType(context, NULL);
    ok(hr == E_INVALIDARG, "expected E_INVALIDARG, got %#x\n", hr);

    type = 0xdeadbeef;
    hr = IWICColorContext_GetType(context, &type);
    ok(hr == S_OK, "GetType error %#x\n", hr);
    ok(type == WICColorContextUninitialized, "unexpected type %u\n", type);

    hr = IWICColorContext_GetProfileBytes(context, 0, NULL, NULL);
    ok(hr == WINCODEC_ERR_NOTINITIALIZED, "GetProfileBytes error %#x\n", hr);

    size = 0;
    hr = IWICColorContext_GetProfileBytes(context, 0, NULL, &size);
    ok(hr == WINCODEC_ERR_NOTINITIALIZED, "GetProfileBytes error %#x\n", hr);
    ok(!size, "unexpected size %u\n", size);

    hr = IWICColorContext_GetExifColorSpace(context, NULL);
    ok(hr == E_INVALIDARG, "expected E_INVALIDARG, got %#x\n", hr);

    colorspace = 0xdeadbeef;
    hr = IWICColorContext_GetExifColorSpace(context, &colorspace);
    ok(hr == S_OK, "GetExifColorSpace error %#x\n", hr);
    ok(colorspace == 0xffffffff, "unexpected color space %u\n", colorspace);

    hr = IWICColorContext_InitializeFromExifColorSpace(context, 0);
    ok(hr == S_OK, "InitializeFromExifColorSpace error %#x\n", hr);

    hr = IWICColorContext_InitializeFromExifColorSpace(context, 1);
    ok(hr == S_OK, "InitializeFromExifColorSpace error %#x\n", hr);

    hr = IWICColorContext_InitializeFromExifColorSpace(context, 2);
    ok(hr == S_OK, "InitializeFromExifColorSpace error %#x\n", hr);

    colorspace = 0xdeadbeef;
    hr = IWICColorContext_GetExifColorSpace(context, &colorspace);
    ok(hr == S_OK, "GetExifColorSpace error %#x\n", hr);
    ok(colorspace == 2, "unexpected color space %u\n", colorspace);

    size = 0;
    hr = IWICColorContext_GetProfileBytes(context, 0, NULL, &size);
    ok(hr == WINCODEC_ERR_NOTINITIALIZED, "GetProfileBytes error %#x\n", hr);
    ok(!size, "unexpected size %u\n", size);

    type = 0xdeadbeef;
    hr = IWICColorContext_GetType(context, &type);
    ok(hr == S_OK, "GetType error %#x\n", hr);
    ok(type == WICColorContextExifColorSpace, "unexpected type %u\n", type);

    hr = IWICBitmapFrameDecode_GetColorContexts(frame, count, &context, &count);
    ok(hr == WINCODEC_ERR_WRONGSTATE, "GetColorContexts error %#x\n", hr);

    IWICColorContext_Release(context);
    IWICBitmapFrameDecode_Release(frame);

    hr = IWICBitmapDecoder_GetFrame(decoder, 0, &frame);
    ok(hr == S_OK, "GetFrame error %#x\n", hr);

    hr = IWICImagingFactory_CreateColorContext(factory, &context);
    ok(hr == S_OK, "CreateColorContext error %#x\n", hr);

    count = 1;
    hr = IWICBitmapFrameDecode_GetColorContexts(frame, count, &context, &count);
    ok(hr == S_OK, "GetColorContexts error %#x\n", hr);

    hr = IWICColorContext_GetProfileBytes(context, 0, NULL, NULL);
    ok(hr == E_INVALIDARG, "expected E_INVALIDARG, got %#x\n", hr);

    size = 0;
    hr = IWICColorContext_GetProfileBytes(context, 0, NULL, &size);
    ok(hr == S_OK, "GetProfileBytes error %#x\n", hr);
    ok(size, "unexpected size %u\n", size);

    buffer = HeapAlloc(GetProcessHeap(), 0, size);
    hr = IWICColorContext_GetProfileBytes(context, size, buffer, &size);
    ok(hr == S_OK, "GetProfileBytes error %#x\n", hr);

    tmpfile = save_profile( buffer, size );
    HeapFree(GetProcessHeap(), 0, buffer);

    type = 0xdeadbeef;
    hr = IWICColorContext_GetType(context, &type);
    ok(hr == S_OK, "GetType error %#x\n", hr);
    ok(type == WICColorContextProfile, "unexpected type %u\n", type);

    colorspace = 0xdeadbeef;
    hr = IWICColorContext_GetExifColorSpace(context, &colorspace);
    ok(hr == S_OK, "GetExifColorSpace error %#x\n", hr);
    ok(colorspace == 0xffffffff, "unexpected color space %u\n", colorspace);

    hr = IWICColorContext_InitializeFromExifColorSpace(context, 1);
    ok(hr == WINCODEC_ERR_WRONGSTATE, "InitializeFromExifColorSpace error %#x\n", hr);

    if (tmpfile)
    {
        hr = IWICColorContext_InitializeFromFilename(context, NULL);
        ok(hr == E_INVALIDARG, "InitializeFromFilename error %#x\n", hr);

        hr = IWICColorContext_InitializeFromFilename(context, tmpfile);
        ok(hr == S_OK, "InitializeFromFilename error %#x\n", hr);

        ret = DeleteFileW(tmpfile);
        ok(ret, "DeleteFileW failed %u\n", GetLastError());

        type = 0xdeadbeef;
        hr = IWICColorContext_GetType(context, &type);
        ok(hr == S_OK, "GetType error %#x\n", hr);
        ok(type == WICColorContextProfile, "unexpected type %u\n", type);

        colorspace = 0xdeadbeef;
        hr = IWICColorContext_GetExifColorSpace(context, &colorspace);
        ok(hr == S_OK, "GetExifColorSpace error %#x\n", hr);
        ok(colorspace == 0xffffffff, "unexpected color space %u\n", colorspace);

        hr = IWICColorContext_InitializeFromExifColorSpace(context, 1);
        ok(hr == WINCODEC_ERR_WRONGSTATE, "InitializeFromExifColorSpace error %#x\n", hr);

        size = 0;
        hr = IWICColorContext_GetProfileBytes(context, 0, NULL, &size);
        ok(hr == S_OK, "GetProfileBytes error %#x\n", hr);
        ok(size, "unexpected size %u\n", size);

        buffer = HeapAlloc(GetProcessHeap(), 0, size);
        hr = IWICColorContext_GetProfileBytes(context, size, buffer, &size);
        ok(hr == S_OK, "GetProfileBytes error %#x\n", hr);

        HeapFree(GetProcessHeap(), 0, buffer);
        HeapFree(GetProcessHeap(), 0, tmpfile);
    }
    IWICColorContext_Release(context);
    IWICBitmapFrameDecode_Release(frame);
    IWICBitmapDecoder_Release(decoder);
}

/* 1 bpp 1x1 pixel PNG image with PLTE and tRNS chunks */
static const char png_PLTE_tRNS[] = {
  0x89,'P','N','G',0x0d,0x0a,0x1a,0x0a,
  0x00,0x00,0x00,0x0d,'I','H','D','R',0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x01,0x01,0x03,0x00,0x00,0x00,0x25,0xdb,0x56,0xca,
  0x00,0x00,0x00,0x06,'P','L','T','E',0x01,0x02,0x03,0x04,0x05,0x06,0x95,0x53,0x6f,0x48,
  0x00,0x00,0x00,0x02,'t','R','N','S',0xff,0x00,0xe5,0xb7,0x30,0x4a,
  0x00,0x00,0x00,0x0a,'I','D','A','T',0x18,0xd3,0x63,0x68,0x00,0x00,0x00,0x82,0x00,0x81,0xa7,0x01,0xba,0x10,
  0x00,0x00,0x00,0x00,'I','E','N','D',0xae,0x42,0x60,0x82
};

static void test_png_palette(void)
{
    HRESULT hr;
    IWICBitmapDecoder *decoder;
    IWICBitmapFrameDecode *frame;
    IWICPalette *palette;
    GUID format;
    UINT count, ret;
    WICColor color[256];

    decoder = create_decoder(png_PLTE_tRNS, sizeof(png_PLTE_tRNS));
    ok(decoder != 0, "Failed to load PNG image data\n");

    hr = IWICBitmapDecoder_GetFrame(decoder, 0, &frame);
    ok(hr == S_OK, "GetFrame error %#x\n", hr);

    hr = IWICBitmapFrameDecode_GetPixelFormat(frame, &format);
    ok(hr == S_OK, "GetPixelFormat error %#x\n", hr);
    ok(IsEqualGUID(&format, &GUID_WICPixelFormat1bppIndexed),
       "got wrong format %s\n", wine_dbgstr_guid(&format));

    hr = IWICImagingFactory_CreatePalette(factory, &palette);
    ok(hr == S_OK, "CreatePalette error %#x\n", hr);
    hr = IWICBitmapFrameDecode_CopyPalette(frame, palette);
    ok(hr == S_OK, "CopyPalette error %#x\n", hr);

    hr = IWICPalette_GetColorCount(palette, &count);
    ok(hr == S_OK, "GetColorCount error %#x\n", hr);
    ok(count == 2, "expected 2, got %u\n", count);

    hr = IWICPalette_GetColors(palette, 256, color, &ret);
    ok(hr == S_OK, "GetColors error %#x\n", hr);
    ok(ret == count, "expected %u, got %u\n", count, ret);
    ok(color[0] == 0xff010203, "expected 0xff010203, got %#x\n", color[0]);
    ok(color[1] == 0x00040506, "expected 0x00040506, got %#x\n", color[1]);

    IWICPalette_Release(palette);
    IWICBitmapFrameDecode_Release(frame);
    IWICBitmapDecoder_Release(decoder);
}

START_TEST(pngformat)
{
    HRESULT hr;

    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    hr = CoCreateInstance(&CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER,
                          &IID_IWICImagingFactory, (void **)&factory);
    ok(hr == S_OK, "CoCreateInstance error %#x\n", hr);
    if (FAILED(hr)) return;

    test_color_contexts();
    test_png_palette();

    IWICImagingFactory_Release(factory);
    CoUninitialize();
}
