//
// This file is part of libcabac.
//
// Copyright 2008 Johannes Ballé <balle@ient.rwth-aachen.de>
//
// libcabac is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libcabac is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libcabac.  If not, see <http://www.gnu.org/licenses/>.
//

#define _OHTU7AY3EI_CABAC_CPP
#include <cabac/common.h>

namespace cabac {

const uint8_t range_tab_lps[ 64 ][ 4 ] = {
  { 128, 176, 208, 240 }, { 128, 167, 197, 227 },
  { 128, 158, 187, 216 }, { 123, 150, 178, 205 },
  { 116, 142, 169, 195 }, { 111, 135, 160, 185 },
  { 105, 128, 152, 175 }, { 100, 122, 144, 166 },
  {  95, 116, 137, 158 }, {  90, 110, 130, 150 },
  {  85, 104, 123, 142 }, {  81,  99, 117, 135 },
  {  77,  94, 111, 128 }, {  73,  89, 105, 122 },
  {  69,  85, 100, 116 }, {  66,  80,  95, 110 },
  {  62,  76,  90, 104 }, {  59,  72,  86,  99 },
  {  56,  69,  81,  94 }, {  53,  65,  77,  89 },
  {  51,  62,  73,  85 }, {  48,  59,  69,  80 },
  {  46,  56,  66,  76 }, {  43,  53,  63,  72 },
  {  41,  50,  59,  69 }, {  39,  48,  56,  65 },
  {  37,  45,  54,  62 }, {  35,  43,  51,  59 },
  {  33,  41,  48,  56 }, {  32,  39,  46,  53 },
  {  30,  37,  43,  50 }, {  29,  35,  41,  48 },
  {  27,  33,  39,  45 }, {  26,  31,  37,  43 },
  {  24,  30,  35,  41 }, {  23,  28,  33,  39 },
  {  22,  27,  32,  37 }, {  21,  26,  30,  35 },
  {  20,  24,  29,  33 }, {  19,  23,  27,  31 },
  {  18,  22,  26,  30 }, {  17,  21,  25,  28 },
  {  16,  20,  23,  27 }, {  15,  19,  22,  25 },
  {  14,  18,  21,  24 }, {  14,  17,  20,  23 },
  {  13,  16,  19,  22 }, {  12,  15,  18,  21 },
  {  12,  14,  17,  20 }, {  11,  14,  16,  19 },
  {  11,  13,  15,  18 }, {  10,  12,  15,  17 },
  {  10,  12,  14,  16 }, {   9,  11,  13,  15 },
  {   9,  11,  12,  14 }, {   8,  10,  12,  14 },
  {   8,   9,  11,  13 }, {   7,   9,  11,  12 },
  {   7,   9,  10,  12 }, {   7,   8,  10,  11 },
  {   6,   8,   9,  11 }, {   6,   7,   9,  10 },
  {   6,   7,   8,   9 }, {   2,   2,   2,   2 },
};

const uint8_t trans_idx_lps[ 64 ] = {
   0,  0,  1,  2,  2,  4,  4,  5,
   6,  7,  8,  9,  9, 11, 11, 12,
  13, 13, 15, 15, 16, 16, 18, 18,
  19, 19, 21, 21, 22, 22, 23, 24,
  24, 25, 26, 26, 27, 27, 28, 29,
  29, 30, 30, 30, 31, 32, 32, 33,
  33, 33, 34, 34, 35, 35, 35, 36,
  36, 36, 37, 37, 37, 38, 38, 63,
};

const uint8_t trans_idx_mps[ 64 ] = {
   1,  2,  3,  4,  5,  6,  7,  8,
   9, 10, 11, 12, 13, 14, 15, 16,
  17, 18, 19, 20, 21, 22, 23, 24,
  25, 26, 27, 28, 29, 30, 31, 32,
  33, 34, 35, 36, 37, 38, 39, 40,
  41, 42, 43, 44, 45, 46, 47, 48,
  49, 50, 51, 52, 53, 54, 55, 56,
  57, 58, 59, 60, 61, 62, 62, 63,
};

const float expect_tab[ 128 ] = {
  1 - PLPS(  0 ), PLPS(  0 ),
  1 - PLPS(  1 ), PLPS(  1 ),
  1 - PLPS(  2 ), PLPS(  2 ),
  1 - PLPS(  3 ), PLPS(  3 ),
  1 - PLPS(  4 ), PLPS(  4 ),
  1 - PLPS(  5 ), PLPS(  5 ),
  1 - PLPS(  6 ), PLPS(  6 ),
  1 - PLPS(  7 ), PLPS(  7 ),
  1 - PLPS(  8 ), PLPS(  8 ),
  1 - PLPS(  9 ), PLPS(  9 ),
  1 - PLPS( 10 ), PLPS( 10 ),
  1 - PLPS( 11 ), PLPS( 11 ),
  1 - PLPS( 12 ), PLPS( 12 ),
  1 - PLPS( 13 ), PLPS( 13 ),
  1 - PLPS( 14 ), PLPS( 14 ),
  1 - PLPS( 15 ), PLPS( 15 ),
  1 - PLPS( 16 ), PLPS( 16 ),
  1 - PLPS( 17 ), PLPS( 17 ),
  1 - PLPS( 18 ), PLPS( 18 ),
  1 - PLPS( 19 ), PLPS( 19 ),
  1 - PLPS( 20 ), PLPS( 20 ),
  1 - PLPS( 21 ), PLPS( 21 ),
  1 - PLPS( 22 ), PLPS( 22 ),
  1 - PLPS( 23 ), PLPS( 23 ),
  1 - PLPS( 24 ), PLPS( 24 ),
  1 - PLPS( 25 ), PLPS( 25 ),
  1 - PLPS( 26 ), PLPS( 26 ),
  1 - PLPS( 27 ), PLPS( 27 ),
  1 - PLPS( 28 ), PLPS( 28 ),
  1 - PLPS( 29 ), PLPS( 29 ),
  1 - PLPS( 30 ), PLPS( 30 ),
  1 - PLPS( 31 ), PLPS( 31 ),
  1 - PLPS( 32 ), PLPS( 32 ),
  1 - PLPS( 33 ), PLPS( 33 ),
  1 - PLPS( 34 ), PLPS( 34 ),
  1 - PLPS( 35 ), PLPS( 35 ),
  1 - PLPS( 36 ), PLPS( 36 ),
  1 - PLPS( 37 ), PLPS( 37 ),
  1 - PLPS( 38 ), PLPS( 38 ),
  1 - PLPS( 39 ), PLPS( 39 ),
  1 - PLPS( 40 ), PLPS( 40 ),
  1 - PLPS( 41 ), PLPS( 41 ),
  1 - PLPS( 42 ), PLPS( 42 ),
  1 - PLPS( 43 ), PLPS( 43 ),
  1 - PLPS( 44 ), PLPS( 44 ),
  1 - PLPS( 45 ), PLPS( 45 ),
  1 - PLPS( 46 ), PLPS( 46 ),
  1 - PLPS( 47 ), PLPS( 47 ),
  1 - PLPS( 48 ), PLPS( 48 ),
  1 - PLPS( 49 ), PLPS( 49 ),
  1 - PLPS( 50 ), PLPS( 50 ),
  1 - PLPS( 51 ), PLPS( 51 ),
  1 - PLPS( 52 ), PLPS( 52 ),
  1 - PLPS( 53 ), PLPS( 53 ),
  1 - PLPS( 54 ), PLPS( 54 ),
  1 - PLPS( 55 ), PLPS( 55 ),
  1 - PLPS( 56 ), PLPS( 56 ),
  1 - PLPS( 57 ), PLPS( 57 ),
  1 - PLPS( 58 ), PLPS( 58 ),
  1 - PLPS( 59 ), PLPS( 59 ),
  1 - PLPS( 60 ), PLPS( 60 ),
  1 - PLPS( 61 ), PLPS( 61 ),
  1 - PLPS( 62 ), PLPS( 62 ),
  1 - PLPS( 63 ), PLPS( 63 ),
};

const uint16_t bits_tab[ 128 ] = {
  FIX8( BITS( 1 - PLPS(  0 ) ) ), FIX8( BITS( PLPS(  0 ) ) ),
  FIX8( BITS( 1 - PLPS(  1 ) ) ), FIX8( BITS( PLPS(  1 ) ) ),
  FIX8( BITS( 1 - PLPS(  2 ) ) ), FIX8( BITS( PLPS(  2 ) ) ),
  FIX8( BITS( 1 - PLPS(  3 ) ) ), FIX8( BITS( PLPS(  3 ) ) ),
  FIX8( BITS( 1 - PLPS(  4 ) ) ), FIX8( BITS( PLPS(  4 ) ) ),
  FIX8( BITS( 1 - PLPS(  5 ) ) ), FIX8( BITS( PLPS(  5 ) ) ),
  FIX8( BITS( 1 - PLPS(  6 ) ) ), FIX8( BITS( PLPS(  6 ) ) ),
  FIX8( BITS( 1 - PLPS(  7 ) ) ), FIX8( BITS( PLPS(  7 ) ) ),
  FIX8( BITS( 1 - PLPS(  8 ) ) ), FIX8( BITS( PLPS(  8 ) ) ),
  FIX8( BITS( 1 - PLPS(  9 ) ) ), FIX8( BITS( PLPS(  9 ) ) ),
  FIX8( BITS( 1 - PLPS( 10 ) ) ), FIX8( BITS( PLPS( 10 ) ) ),
  FIX8( BITS( 1 - PLPS( 11 ) ) ), FIX8( BITS( PLPS( 11 ) ) ),
  FIX8( BITS( 1 - PLPS( 12 ) ) ), FIX8( BITS( PLPS( 12 ) ) ),
  FIX8( BITS( 1 - PLPS( 13 ) ) ), FIX8( BITS( PLPS( 13 ) ) ),
  FIX8( BITS( 1 - PLPS( 14 ) ) ), FIX8( BITS( PLPS( 14 ) ) ),
  FIX8( BITS( 1 - PLPS( 15 ) ) ), FIX8( BITS( PLPS( 15 ) ) ),
  FIX8( BITS( 1 - PLPS( 16 ) ) ), FIX8( BITS( PLPS( 16 ) ) ),
  FIX8( BITS( 1 - PLPS( 17 ) ) ), FIX8( BITS( PLPS( 17 ) ) ),
  FIX8( BITS( 1 - PLPS( 18 ) ) ), FIX8( BITS( PLPS( 18 ) ) ),
  FIX8( BITS( 1 - PLPS( 19 ) ) ), FIX8( BITS( PLPS( 19 ) ) ),
  FIX8( BITS( 1 - PLPS( 20 ) ) ), FIX8( BITS( PLPS( 20 ) ) ),
  FIX8( BITS( 1 - PLPS( 21 ) ) ), FIX8( BITS( PLPS( 21 ) ) ),
  FIX8( BITS( 1 - PLPS( 22 ) ) ), FIX8( BITS( PLPS( 22 ) ) ),
  FIX8( BITS( 1 - PLPS( 23 ) ) ), FIX8( BITS( PLPS( 23 ) ) ),
  FIX8( BITS( 1 - PLPS( 24 ) ) ), FIX8( BITS( PLPS( 24 ) ) ),
  FIX8( BITS( 1 - PLPS( 25 ) ) ), FIX8( BITS( PLPS( 25 ) ) ),
  FIX8( BITS( 1 - PLPS( 26 ) ) ), FIX8( BITS( PLPS( 26 ) ) ),
  FIX8( BITS( 1 - PLPS( 27 ) ) ), FIX8( BITS( PLPS( 27 ) ) ),
  FIX8( BITS( 1 - PLPS( 28 ) ) ), FIX8( BITS( PLPS( 28 ) ) ),
  FIX8( BITS( 1 - PLPS( 29 ) ) ), FIX8( BITS( PLPS( 29 ) ) ),
  FIX8( BITS( 1 - PLPS( 30 ) ) ), FIX8( BITS( PLPS( 30 ) ) ),
  FIX8( BITS( 1 - PLPS( 31 ) ) ), FIX8( BITS( PLPS( 31 ) ) ),
  FIX8( BITS( 1 - PLPS( 32 ) ) ), FIX8( BITS( PLPS( 32 ) ) ),
  FIX8( BITS( 1 - PLPS( 33 ) ) ), FIX8( BITS( PLPS( 33 ) ) ),
  FIX8( BITS( 1 - PLPS( 34 ) ) ), FIX8( BITS( PLPS( 34 ) ) ),
  FIX8( BITS( 1 - PLPS( 35 ) ) ), FIX8( BITS( PLPS( 35 ) ) ),
  FIX8( BITS( 1 - PLPS( 36 ) ) ), FIX8( BITS( PLPS( 36 ) ) ),
  FIX8( BITS( 1 - PLPS( 37 ) ) ), FIX8( BITS( PLPS( 37 ) ) ),
  FIX8( BITS( 1 - PLPS( 38 ) ) ), FIX8( BITS( PLPS( 38 ) ) ),
  FIX8( BITS( 1 - PLPS( 39 ) ) ), FIX8( BITS( PLPS( 39 ) ) ),
  FIX8( BITS( 1 - PLPS( 40 ) ) ), FIX8( BITS( PLPS( 40 ) ) ),
  FIX8( BITS( 1 - PLPS( 41 ) ) ), FIX8( BITS( PLPS( 41 ) ) ),
  FIX8( BITS( 1 - PLPS( 42 ) ) ), FIX8( BITS( PLPS( 42 ) ) ),
  FIX8( BITS( 1 - PLPS( 43 ) ) ), FIX8( BITS( PLPS( 43 ) ) ),
  FIX8( BITS( 1 - PLPS( 44 ) ) ), FIX8( BITS( PLPS( 44 ) ) ),
  FIX8( BITS( 1 - PLPS( 45 ) ) ), FIX8( BITS( PLPS( 45 ) ) ),
  FIX8( BITS( 1 - PLPS( 46 ) ) ), FIX8( BITS( PLPS( 46 ) ) ),
  FIX8( BITS( 1 - PLPS( 47 ) ) ), FIX8( BITS( PLPS( 47 ) ) ),
  FIX8( BITS( 1 - PLPS( 48 ) ) ), FIX8( BITS( PLPS( 48 ) ) ),
  FIX8( BITS( 1 - PLPS( 49 ) ) ), FIX8( BITS( PLPS( 49 ) ) ),
  FIX8( BITS( 1 - PLPS( 50 ) ) ), FIX8( BITS( PLPS( 50 ) ) ),
  FIX8( BITS( 1 - PLPS( 51 ) ) ), FIX8( BITS( PLPS( 51 ) ) ),
  FIX8( BITS( 1 - PLPS( 52 ) ) ), FIX8( BITS( PLPS( 52 ) ) ),
  FIX8( BITS( 1 - PLPS( 53 ) ) ), FIX8( BITS( PLPS( 53 ) ) ),
  FIX8( BITS( 1 - PLPS( 54 ) ) ), FIX8( BITS( PLPS( 54 ) ) ),
  FIX8( BITS( 1 - PLPS( 55 ) ) ), FIX8( BITS( PLPS( 55 ) ) ),
  FIX8( BITS( 1 - PLPS( 56 ) ) ), FIX8( BITS( PLPS( 56 ) ) ),
  FIX8( BITS( 1 - PLPS( 57 ) ) ), FIX8( BITS( PLPS( 57 ) ) ),
  FIX8( BITS( 1 - PLPS( 58 ) ) ), FIX8( BITS( PLPS( 58 ) ) ),
  FIX8( BITS( 1 - PLPS( 59 ) ) ), FIX8( BITS( PLPS( 59 ) ) ),
  FIX8( BITS( 1 - PLPS( 60 ) ) ), FIX8( BITS( PLPS( 60 ) ) ),
  FIX8( BITS( 1 - PLPS( 61 ) ) ), FIX8( BITS( PLPS( 61 ) ) ),
  FIX8( BITS( 1 - PLPS( 62 ) ) ), FIX8( BITS( PLPS( 62 ) ) ),
  FIX8( BITS( 1 - PLPS( 63 ) ) ), FIX8( BITS( PLPS( 63 ) ) ),
};

}
