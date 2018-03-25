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

final class DictSpec<Tk as arraykey, Tv> extends TypeSpec<dict<Tk, Tv>> {

  public function __construct(
    private TypeSpec<Tk> $tsk,
    private TypeSpec<Tv> $tsv,
  ) {
  }

  public function coerceType(mixed $value): dict<Tk, Tv> {
    if (!$value instanceof KeyedTraversable) {
      throw TypeCoercionException::withValue(
        $this->getTrace(),
        'dict<Tk, Tv>',
        $value,
      );
    }

    $kt = $this->getTrace()->withFrame('dict<Tk, _>');
    $vt = $this->getTrace()->withFrame('dict<_, Tv>');

    return Dict\pull_with_key(
      $value,
      ($_k, $v) ==> $this->tsv->withTrace($vt)->coerceType($v),
      ($k, $_v) ==> $this->tsk->withTrace($kt)->coerceType($k),
    );
  }

  public function assertType(mixed $value): dict<Tk, Tv> {
    if (!is_dict($value)) {
      throw TypeCoercionException::withValue(
        $this->getTrace(),
        'dict<Tk, Tv>',
        $value,
      );
    }

    $kt = $this->getTrace()->withFrame('dict<Tk, _>');
    $vt = $this->getTrace()->withFrame('dict<_, Tv>');

    return Dict\pull_with_key(
      $value,
      ($_k, $v) ==> $this->tsv->withTrace($vt)->assertType($v),
      ($k, $_v) ==> $this->tsk->withTrace($kt)->assertType($k),
    );
  }
}
