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

namespace HH\Lib\Vec;

/**
 * Returns a vec containing the original vec split into chunks of the given
 * size. If the original vec doesn't divide evenly, the final chunk will be
 * smaller.
 */
function chunk<Tv>(
  Traversable<Tv> $traversable,
  int $size,
): vec<vec<Tv>> {
  invariant($size > 0, 'Expected positive chunk size, got %d.', $size);
  $result = vec[];
  $ii = 0;
  foreach ($traversable as $value) {
    if ($ii % $size === 0) {
      $result[] = vec[];
    }
    $result[\intdiv($ii, $size)][] = $value;
    $ii++;
  }
  return $result;
}

/**
 * Returns a new vec of size `$size` where all the values are `$value`.
 */
function fill<Tv>(
  int $size,
  Tv $value,
): vec<Tv> {
  invariant($size >= 0, 'Expected non-negative fill size, got %d.', $size);
  return vec(\array_fill(0, $size, $value));
}

/**
 * Returns a new vec formed by joining the Traversable elements of the given
 * Traversable.
 *
 * For a fixed number of Traversables, see `Vec\concat()`.
 */
function flatten<Tv>(
  Traversable<Traversable<Tv>> $traversables,
): vec<Tv> {
  $result = vec[];
  foreach ($traversables as $traversable) {
    foreach ($traversable as $value) {
      $result[] = $value;
    }
  }
  return $result;
}

/**
 * Returns a new vec where each value is the result of calling the given
 * function on the original value.
 *
 * For async functions, see `Vec\map_async()`.
 */
function map<Tv1, Tv2>(
  Traversable<Tv1> $traversable,
  (function(Tv1): Tv2) $value_func,
): vec<Tv2> {
  $result = vec[];
  foreach ($traversable as $value) {
    $result[] = $value_func($value);
  }
  return $result;
}

/**
 * Returns a new vec where each value is the result of calling the given
 * function on the original key and value.
 */
function map_with_key<Tk, Tv1, Tv2>(
  KeyedTraversable<Tk, Tv1> $traversable,
  (function(Tk, Tv1): Tv2) $value_func,
): vec<Tv2> {
  $result = vec[];
  foreach ($traversable as $key => $value) {
    $result[] = $value_func($key, $value);
  }
  return $result;
}
