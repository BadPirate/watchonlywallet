<?hh // strict
/*
 *  Copyright (c) 2015, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */

class :ins extends :xhp:html-element {
  attribute
    Stringish cite,
    Stringish datetime;
  category %flow, %phrase;
  children (pcdata | %flow)*;
  protected string $tagName = 'ins';
}
