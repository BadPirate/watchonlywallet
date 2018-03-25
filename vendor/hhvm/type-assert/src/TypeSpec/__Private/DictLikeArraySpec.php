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

use namespace HH\Lib\Dict;

final class DictLikeArraySpec<Tk as arraykey, Tv>
  extends TypeSpec<array<Tk, Tv>> {

  public function __construct(
    private TypeSpec<Tk> $tsk,
    private TypeSpec<Tv> $tsv,
  ) {
  }

  public function coerceType(mixed $value): array<Tk, Tv> {
    if (!$value instanceof KeyedTraversable) {
      throw TypeCoercionException::withValue(
        $this->getTrace(),
        'array<Tk, Tv>',
        $value,
      );
    }

    $kt = $this->getTrace()->withFrame('array<Tk, _>');
    $vt = $this->getTrace()->withFrame('array<_, Tv>');

    return Dict\pull_with_key(
      $value,
      ($_k, $v) ==> $this->tsv->withTrace($vt)->coerceType($v),
      ($k, $_v) ==> $this->tsk->withTrace($kt)->coerceType($k),
    )
      |> /* HH_IGNORE_ERROR[4007] PHP array cast */ (array)$$;
  }

  public function assertType(mixed $value): array<Tk, Tv> {
    if (!is_array($value)) {
      throw IncorrectTypeException::withValue(
        $this->getTrace(),
        'array<Tk, Tv>',
        $value,
      );
    }

    $kt = $this->getTrace()->withFrame('array<Tk, _>');
    $vt = $this->getTrace()->withFrame('array<_, Tv>');

    return Dict\pull_with_key(
      $value,
      ($_k, $v) ==> $this->tsv->withTrace($vt)->assertType($v),
      ($k, $_v) ==> $this->tsk->withTrace($kt)->assertType($k),
    )
      |> /* HH_IGNORE_ERROR[4007] PHP array cast */ (array)$$;
  }
}
