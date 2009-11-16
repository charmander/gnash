// action.cpp:  ActionScript execution, for Gnash.
//
//   Copyright (C) 2005, 2006, 2007, 2008, 2009 Free Software Foundation, Inc.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//


#include "action.h"
#include "as_object.h"
#include "log.h"
#include "as_function.h"
#include "SWF.h"
#include "GnashException.h"
#include "as_environment.h"
#include "fn_call.h"
#include "event_id.h"
#include "VM.h"
#include "StringPredicates.h"
#include "namedStrings.h"

#include <string>
#include <algorithm>
#include <boost/format.hpp>
#include <cassert>
#include <boost/assign/list_of.hpp>

namespace gnash {

//
// action stuff
//

//
// Function/method dispatch.
//

/// @param this_ptr     this is ourself.
as_value
call_method(const as_value& method, const as_environment& env,
        as_object* this_ptr, fn_call::Args& args, as_object* super,
        const movie_definition* callerDef)
{
	as_value val;
	fn_call call(this_ptr, env, args);
	call.super = super;
    call.callerDef = callerDef;

	try {
		if (as_function* func = method.to_as_function()) {
            // Call function.
		    val = (*func)(call);
		}
		else {
            IF_VERBOSE_ASCODING_ERRORS(
                log_aserror("Attempt to call a value which is not "
                    "a function (%s)", method);
            );
            return val;
		}
	}
	catch (ActionTypeError& e) {
		assert(val.is_undefined());
		IF_VERBOSE_ASCODING_ERRORS(
            log_aserror("%s", e.what());
		);
	}

	return val;
}

as_value
call_method0(const as_value& method, const as_environment& env,
        as_object* this_ptr)
{
    fn_call::Args args;
    return call_method(method, env, this_ptr, args);
}


//
// event_id
//

const std::string&
event_id::functionName() const
{
    typedef std::map<EventCode, std::string> EventFunctionNameMap;
    static const EventFunctionNameMap e = boost::assign::map_list_of
        (INVALID, "INVALID")
		(PRESS, "onPress")
		(RELEASE, "onRelease")
		(RELEASE_OUTSIDE, "onReleaseOutside")
		(ROLL_OVER, "onRollOver")
		(ROLL_OUT, "onRollOut")	
		(DRAG_OVER, "onDragOver")
		(DRAG_OUT, "onDragOut")	
		(KEY_PRESS, "onKeyPress")
		(INITIALIZE, "onInitialize")
		(LOAD, "onLoad")
		(UNLOAD, "onUnload")
		(ENTER_FRAME, "onEnterFrame")
		(MOUSE_DOWN, "onMouseDown")	
		(MOUSE_UP, "onMouseUp")
		(MOUSE_MOVE, "onMouseMove")
		(KEY_DOWN, "onKeyDown")
		(KEY_UP, "onKeyUp")	
		(DATA, "onData")
		(CONSTRUCT, "onConstruct");

    EventFunctionNameMap::const_iterator it = e.find(_id);
    assert(it != e.end());
    return it->second;
}

string_table::key
event_id::functionKey() const
{
    typedef std::map<EventCode, string_table::key> EventFunctionMap;
    static const EventFunctionMap e = boost::assign::map_list_of
		(PRESS, NSV::PROP_ON_PRESS)
		(RELEASE, NSV::PROP_ON_RELEASE)
		(RELEASE_OUTSIDE, NSV::PROP_ON_RELEASE_OUTSIDE)
		(ROLL_OVER, NSV::PROP_ON_ROLL_OVER )
		(ROLL_OUT, NSV::PROP_ON_ROLL_OUT)
		(DRAG_OVER, NSV::PROP_ON_DRAG_OVER)
		(DRAG_OUT, NSV::PROP_ON_DRAG_OUT)
		(KEY_PRESS, NSV::PROP_ON_KEY_PRESS)
		(INITIALIZE, NSV::PROP_ON_INITIALIZE)
		(LOAD, NSV::PROP_ON_LOAD)
		(UNLOAD, NSV::PROP_ON_UNLOAD)
		(ENTER_FRAME, NSV::PROP_ON_ENTER_FRAME)
		(MOUSE_DOWN, NSV::PROP_ON_MOUSE_DOWN)
		(MOUSE_UP, NSV::PROP_ON_MOUSE_UP)
		(MOUSE_MOVE, NSV::PROP_ON_MOUSE_MOVE)
		(KEY_DOWN, NSV::PROP_ON_KEY_DOWN)
		(KEY_UP, NSV::PROP_ON_KEY_UP)
		(DATA, NSV::PROP_ON_DATA)
		(CONSTRUCT, NSV::PROP_ON_CONSTRUCT);

    EventFunctionMap::const_iterator it = e.find(_id);
    assert(it != e.end());
    return it->second;
}

bool
event_id::is_mouse_event() const
{
	switch (_id)
	{
		case event_id::PRESS:
		case event_id::RELEASE:
		case event_id::RELEASE_OUTSIDE:
		case event_id::MOUSE_UP:
		case event_id::MOUSE_DOWN:
		case event_id::ROLL_OVER:
		case event_id::ROLL_OUT:
		case event_id::DRAG_OVER:
		case event_id::DRAG_OUT:
			return true;
		default:
			return false;
	}
}

bool
event_id::is_key_event() const
{
	switch (_id)
	{
		case event_id::KEY_DOWN:
		case event_id::KEY_PRESS:
		case event_id::KEY_UP:
			return true;
		default:
			return false;
	}
}

bool
event_id::is_button_event() const
{
	switch (_id)
	{
		case event_id::PRESS:
		case event_id::RELEASE:
		case event_id::RELEASE_OUTSIDE:
		case event_id::ROLL_OVER:
		case event_id::ROLL_OUT:
		case event_id::DRAG_OVER:
		case event_id::DRAG_OUT:
		case event_id::KEY_PRESS:
			return true;
		default:
			return false;
	}
}

std::ostream& operator<< (std::ostream& o, const event_id& ev)
{
    return (o << ev.functionName());
}

} // end of namespace gnash

// Local Variables:
// mode: C++
// indent-tabs-mode: t
// End:
