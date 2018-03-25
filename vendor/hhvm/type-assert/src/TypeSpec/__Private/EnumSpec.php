<?hh // strict
/*
 * Copyright (c) 2017, Facebook Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

namespace Facebook\TypeSpec\__Private;

use type Facebook\TypeAssert\{IncorrectTypeException, TypeCoercionException};
use type Facebook\TypeSpec\TypeSpec;

final class
  EnumSpec<
    Tinner,
    T as /* HH_IGNORE_ERROR[2053] */ \HH\BuiltinEnum<Tinner>,
  > extends TypeSpec<T> {
  public function __construct(private classname<T> $what) {
  }

  public function coerceType(mixed $value): T {
    $what = $this->what;
    try {
      /* HH_IGNORE_ERROR[4110] */
      return $what::assert($value);
    } catch (\UnexpectedValueException $_e) {
      throw TypeCoercionException::withValue($this->getTrace(), $what, $value);
    }
  }

  public function assertType(mixed $value): T {
    $what = $this->what;
    try {
      /* HH_IGNORE_ERROR[4110] */
      return $what::assert($value);
    } catch (\UnexpectedValueException $_e) {
      throw IncorrectTypeException::withValue($this->getTrace(), $what, $value);
    }
  }
}
