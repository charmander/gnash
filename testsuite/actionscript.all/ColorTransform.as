// 
//   Copyright (C) 2008 Free Software Foundation, Inc.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
//
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
// Test case for TextFormat ActionScript class
// compile this test case with Ming makeswf, and then
// execute it like this gnash -1 -r 0 -v out.swf

rcsid="$Id: ColorTransform.as,v 1.11 2008/06/19 11:49:17 bwy Exp $";

#include "check.as"

#if OUTPUT_VERSION < 8

check_equals(typeof(flash), 'undefined');

check_totals(1);

#else

ColorTransform = flash.geom.ColorTransform;
check_equals(typeof(ColorTransform), 'function');
check_equals(typeof(ColorTransform.prototype), 'object');
check(ColorTransform.prototype.hasOwnProperty('rgb'));
check(ColorTransform.prototype.hasOwnProperty('toString'));
check(ColorTransform.prototype.hasOwnProperty('concat'));
check(ColorTransform.prototype.hasOwnProperty('redMultiplier'));
check(ColorTransform.prototype.hasOwnProperty('blueMultiplier'));
check(ColorTransform.prototype.hasOwnProperty('greenMultiplier'));
check(ColorTransform.prototype.hasOwnProperty('alphaMultiplier'));
check(ColorTransform.prototype.hasOwnProperty('redOffset'));
check(ColorTransform.prototype.hasOwnProperty('blueOffset'));
check(ColorTransform.prototype.hasOwnProperty('greenOffset'));
check(ColorTransform.prototype.hasOwnProperty('alphaOffset'));

//-------------------------------------------------------------
// Test constructor
//-------------------------------------------------------------

c = new ColorTransform;
check_equals(typeof(c), 'object');
check(c instanceof ColorTransform);

xcheck_equals(c.redMultiplier, 1);
xcheck_equals(c.blueMultiplier, 1);
xcheck_equals(c.greenMultiplier, 1);
xcheck_equals(c.alphaMultiplier, 1);
xcheck_equals(c.redOffset, 0);
xcheck_equals(c.blueOffset, 0);
xcheck_equals(c.greenOffset, 0);
xcheck_equals(c.alphaOffset, 0);

xcheck_equals (c.toString(), "(redMultiplier=1, greenMultiplier=1, blueMultiplier=1, alphaMultiplier=1, redOffset=0, greenOffset=0, blueOffset=0, alphaOffset=0)");

c.redMultiplier = 5.4;
xcheck_equals(c.redMultiplier.toString(), "5.4");

c.alphaMultiplier = -0.3;
xcheck_equals(c.alphaMultiplier.toString(), "-0.3");

c.redOffset = 123;
xcheck_equals(c.redOffset.toString(), "123");

c.greenOffset = 287;
xcheck_equals(c.greenOffset.toString(), "287");

o = {};
o.valueOf = function() { return 456; };

c = new ColorTransform(new Object, 3, "string", true, ASnative(100,9), new Error("custom error"), undefined, o);

xcheck_equals (c.toString(), "(redMultiplier=NaN, greenMultiplier=3, blueMultiplier=NaN, alphaMultiplier=1, redOffset=NaN, greenOffset=NaN, blueOffset=NaN, alphaOffset=456)");

//-------------------------------------------------------------
// END OF TEST
//-------------------------------------------------------------

totals(29);

#endif // OUTPUT_VERSION >= 8
