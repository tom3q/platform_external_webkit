/*
 * Copyright (C) 2010 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef WebIDBCursor_h
#define WebIDBCursor_h

#include "WebCommon.h"
#include "WebIDBCallbacks.h"
#include "WebIDBKey.h"
#include "WebSerializedScriptValue.h"
#include "WebString.h"

namespace WebKit {

// See comment in WebIndexedDatabase for a high level overview these classes.
class WebIDBCursor {
public:
    virtual ~WebIDBCursor() { }

    virtual unsigned short direction() const
    {
        WEBKIT_ASSERT_NOT_REACHED();
        return 0;
    }
    virtual WebIDBKey key() const
    {
        WEBKIT_ASSERT_NOT_REACHED();
        return WebIDBKey::createInvalid();
    }
    // FIXME: Remove the legacy version of value() (the first version) as soon as a WebKit roll happens.
    virtual WebSerializedScriptValue value() const
    {
        WebSerializedScriptValue serializedScriptValue;
        WebIDBKey idbKey;
        value(serializedScriptValue, idbKey);
        WEBKIT_ASSERT(!serializedScriptValue.isNull());
        WEBKIT_ASSERT(idbKey.type() == WebIDBKey::InvalidType);
        return serializedScriptValue;
    }
    // One or the other will set, depending on what type of cursor this is.
    virtual void value(WebSerializedScriptValue& serializedScriptValue, WebIDBKey& idbKey) const
    {
        serializedScriptValue = value();
    }

    virtual void update(const WebSerializedScriptValue&, WebIDBCallbacks*) { WEBKIT_ASSERT_NOT_REACHED(); }
    virtual void continueFunction(const WebIDBKey&, WebIDBCallbacks*) { WEBKIT_ASSERT_NOT_REACHED(); }
    virtual void remove(WebIDBCallbacks*) { WEBKIT_ASSERT_NOT_REACHED(); }
};

} // namespace WebKit

#endif // WebIDBCursor_h