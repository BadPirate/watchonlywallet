<?hh // strict
/*
 *  Copyright (c) 2004-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */

namespace HH\Lib\Keyset;

/**
 * Returns a new keyset containing all of the elements of the given
 * Traversables.
 *
 * For a variable number of Traversables, see `Keyset\flatten()`.
 */
function union<Tv as arraykey>(
  Traversable<Tv> $first,
  Traversable<Tv> ...$rest
): keyset<Tv> {
  $result = keyset($first);
  foreach ($rest as $traversable) {
    foreach ($traversable as $value) {
      $result[] = $value;
    }
  }
  return $result;
}
