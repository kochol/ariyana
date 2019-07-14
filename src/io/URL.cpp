#include "URL.hpp"
#include "core/string/StringBuilder.hpp"
#include "core/log.h"
#include "FileSystem.hpp"

namespace ari::io
{
//------------------------------------------------------------------------------
void URL::clearIndices()
{
    for (int i = 0; i < numIndices; i++)
    {
        this->indices[i] = core::InvalidIndex;
    }
}

//------------------------------------------------------------------------------
void URL::copyIndices(const URL &rhs)
{
    for (int i = 0; i < numIndices; i++)
    {
        this->indices[i] = rhs.indices[i];
    }
}

//------------------------------------------------------------------------------
URL::URL() : valid(false)
{
    this->clearIndices();
}

//------------------------------------------------------------------------------
URL::URL(const URL &rhs) : content(rhs.content),
                           valid(rhs.valid)
{
    this->copyIndices(rhs);
}

//------------------------------------------------------------------------------
URL::URL(URL &&rhs)
{
    this->content = std::move(rhs.content);
    this->copyIndices(rhs);
    this->valid = rhs.valid;
    rhs.clearIndices();
    rhs.valid = false;
}

//------------------------------------------------------------------------------
URL::URL(const char *rhs) : valid(false)
{
    this->crack(rhs);
}

//------------------------------------------------------------------------------
URL::URL(const core::StringAtom &rhs) : valid(false)
{
    this->crack(rhs.AsString());
}

//------------------------------------------------------------------------------
URL::URL(const core::String &rhs) : content(rhs),
                                    valid(false)
{
    this->crack(rhs);
}

//------------------------------------------------------------------------------
void URL::operator=(const URL &rhs)
{
    if (&rhs != this)
    {
        this->content = rhs.content;
        this->copyIndices(rhs);
        this->valid = rhs.valid;
    }
}

//------------------------------------------------------------------------------
void URL::operator=(URL &&rhs)
{
    if (&rhs != this)
    {
        this->content = rhs.content;
        this->copyIndices(rhs);
        this->valid = rhs.valid;
        rhs.content.Clear();
        rhs.clearIndices();
        rhs.valid = false;
    }
}

//------------------------------------------------------------------------------
void URL::operator=(const char *rhs)
{
    this->content = rhs;
    this->crack(rhs);
}

//------------------------------------------------------------------------------
void URL::operator=(const core::StringAtom &rhs)
{
    this->content = rhs;
    this->crack(rhs.AsString());
}

//------------------------------------------------------------------------------
void URL::operator=(const core::String &rhs)
{
    this->content = rhs;
    this->crack(rhs);
}

//------------------------------------------------------------------------------
bool URL::operator==(const URL &rhs)
{
    return this->content == rhs.content;
}

//------------------------------------------------------------------------------
bool URL::operator!=(const URL &rhs)
{
    return this->content != rhs.content;
}

//------------------------------------------------------------------------------
const core::StringAtom &
URL::Get() const
{
    return this->content;
}

//------------------------------------------------------------------------------
const char *
URL::AsCStr() const
{
    return this->content.AsCStr();
}

//------------------------------------------------------------------------------
bool URL::IsValid() const
{
    return this->valid;
}

//------------------------------------------------------------------------------
bool URL::Empty() const
{
    return !this->content.IsValid();
}

//------------------------------------------------------------------------------
void URL::crack(core::String urlString)
{

    this->content.Clear();
    this->clearIndices();
    this->valid = false;

    urlString = ResolveAssigns(urlString);
    if (urlString.IsValid())
    {

        core::StringBuilder builder;
        builder.Set(urlString);
        this->content = urlString;

        // extract scheme
        this->indices[schemeStart] = 0;
        this->indices[schemeEnd] = builder.FindSubString(0, 8, "://");
        if (core::EndOfString == this->indices[schemeEnd])
        {
            log_warn("URL::crack(): '%s' is not a valid URL!\n", this->content.AsCStr());
            this->clearIndices();
            return;
        }

        // extract host fields
        int leftStartIndex = this->indices[schemeEnd] + 3;
        int leftEndIndex = builder.FindFirstOf(leftStartIndex, core::EndOfString, "/");
        if (core::EndOfString == leftEndIndex)
        {
            leftEndIndex = builder.Length();
        }
        if (leftStartIndex != leftEndIndex)
        {
            // extract user and password
            int userAndPwdEndIndex = builder.FindFirstOf(leftStartIndex, leftEndIndex, "@");
            if (core::EndOfString != userAndPwdEndIndex)
            {
                // only user, or user:pwd?
                int userEndIndex = builder.FindFirstOf(leftStartIndex, userAndPwdEndIndex, ":");
                if (core::EndOfString != userEndIndex)
                {
                    // user and password
                    this->indices[userStart] = leftStartIndex;
                    this->indices[userEnd] = userEndIndex;
                    this->indices[pwdStart] = userEndIndex + 1;
                    this->indices[pwdEnd] = userAndPwdEndIndex;
                }
                else
                {
                    // only user
                    this->indices[userStart] = leftStartIndex;
                    this->indices[userEnd] = userAndPwdEndIndex;
                }
                leftStartIndex = userAndPwdEndIndex + 1;
            }

            // extract host and port
            int hostEndIndex = builder.FindFirstOf(leftStartIndex, leftEndIndex, ":");
            if (core::EndOfString != hostEndIndex)
            {
                // host and port
                this->indices[hostStart] = leftStartIndex;
                this->indices[hostEnd] = hostEndIndex;
                this->indices[portStart] = hostEndIndex + 1;
                this->indices[portEnd] = leftEndIndex;
            }
            else
            {
                // only host name, no port
                this->indices[hostStart] = leftStartIndex;
                this->indices[hostEnd] = leftEndIndex;
            }
        }

        // is there any path component?
        if (leftEndIndex != builder.Length())
        {
            // extract right-hand-side (path, fragment, query)
            int rightStartIndex = leftEndIndex + 1;
            int rightEndIndex = builder.Length();

            int pathStartIndex = rightStartIndex;
            int pathEndIndex = builder.FindFirstOf(rightStartIndex, rightEndIndex, "#?");
            if (core::EndOfString == pathEndIndex)
            {
                pathEndIndex = rightEndIndex;
            }
            if (pathStartIndex != pathEndIndex)
            {
                this->indices[pathStart] = pathStartIndex;
                this->indices[pathEnd] = pathEndIndex;
            }

            // extract query
            if ((pathEndIndex != rightEndIndex) && (builder.At(pathEndIndex) == '?'))
            {
                int queryStartIndex = pathEndIndex + 1;
                int queryEndIndex = builder.FindFirstOf(queryStartIndex, rightEndIndex, "#");
                if (core::EndOfString == queryEndIndex)
                {
                    queryEndIndex = rightEndIndex;
                }
                if (queryStartIndex != queryEndIndex)
                {
                    this->indices[queryStart] = queryStartIndex;
                    this->indices[queryEnd] = queryEndIndex;
                }
                pathEndIndex = queryEndIndex;
            }

            // extract fragment
            if ((pathEndIndex != rightEndIndex) && (builder.At(pathEndIndex) == '#'))
            {
                int fragStartIndex = pathEndIndex + 1;
                int fragEndIndex = builder.FindFirstOf(fragStartIndex, rightEndIndex, "?");
                if (core::EndOfString == fragEndIndex)
                {
                    fragEndIndex = rightEndIndex;
                }
                if (fragStartIndex != fragEndIndex)
                {
                    this->indices[fragStart] = fragStartIndex;
                    this->indices[fragEnd] = fragEndIndex;
                }
            }
        }
        this->valid = true;
    }
    // fallthrough if valid or empty URL
}

//------------------------------------------------------------------------------
bool URL::HasScheme() const
{
    return core::InvalidIndex != this->indices[schemeStart];
}

//------------------------------------------------------------------------------
core::String
URL::Scheme() const
{
    if (this->HasScheme())
    {
        return core::String(this->content.AsCStr(), this->indices[schemeStart], this->indices[schemeEnd]);
    }
    else
    {
        return core::String();
    }
}

//------------------------------------------------------------------------------
bool URL::HasUser() const
{
    return core::InvalidIndex != this->indices[userStart];
}

//------------------------------------------------------------------------------
core::String
URL::User() const
{
    if (this->HasUser())
    {
        return core::String(this->content.AsCStr(), this->indices[userStart], this->indices[userEnd]);
    }
    else
    {
        return core::String();
    }
}

//------------------------------------------------------------------------------
bool URL::HasPassword() const
{
    return core::InvalidIndex != this->indices[pwdStart];
}

//------------------------------------------------------------------------------
core::String
URL::Password() const
{
    if (this->HasPassword())
    {
        return core::String(this->content.AsCStr(), this->indices[pwdStart], this->indices[pwdEnd]);
    }
    else
    {
        return core::String();
    }
}

//------------------------------------------------------------------------------
bool URL::HasHost() const
{
    return core::InvalidIndex != this->indices[hostStart];
}

//------------------------------------------------------------------------------
core::String
URL::Host() const
{
    if (this->HasHost())
    {
        return core::String(this->content.AsCStr(), this->indices[hostStart], this->indices[hostEnd]);
    }
    else
    {
        return core::String();
    }
}

//------------------------------------------------------------------------------
bool URL::HasPort() const
{
    return core::InvalidIndex != this->indices[portStart];
}

//------------------------------------------------------------------------------
core::String
URL::Port() const
{
    if (this->HasPort())
    {
        return core::String(this->content.AsCStr(), this->indices[portStart], this->indices[portEnd]);
    }
    else
    {
        return core::String();
    }
}

//------------------------------------------------------------------------------
core::String
URL::HostAndPort() const
{
    if (this->HasHost())
    {
        if (this->HasPort())
        {
            // URL has host and port definition
            return core::String(this->content.AsCStr(), this->indices[hostStart], this->indices[portEnd]);
        }
        else
        {
            // URL only has host
            return core::String(this->content.AsCStr(), this->indices[hostStart], this->indices[hostEnd]);
        }
    }
    else
    {
        // no host in URL
        return core::String();
    }
}

//------------------------------------------------------------------------------
bool URL::HasPath() const
{
    return core::InvalidIndex != this->indices[pathStart];
}

//------------------------------------------------------------------------------
core::String
URL::Path() const
{
    if (this->HasPath())
    {
        return core::String(this->content.AsCStr(), this->indices[pathStart], this->indices[pathEnd]);
    }
    else
    {
        return core::String();
    }
}

//------------------------------------------------------------------------------
bool URL::HasFragment() const
{
    return core::InvalidIndex != this->indices[fragStart];
}

//------------------------------------------------------------------------------
core::String
URL::Fragment() const
{
    if (this->HasFragment())
    {
        return core::String(this->content.AsCStr(), this->indices[fragStart], this->indices[fragEnd]);
    }
    else
    {
        return core::String();
    }
}

//------------------------------------------------------------------------------
core::String
URL::PathToEnd() const
{
    if (this->HasPath())
    {
        return core::String(this->content.AsCStr(), this->indices[pathStart], core::EndOfString);
    }
    else
    {
        return core::String();
    }
}

//------------------------------------------------------------------------------
bool URL::HasQuery() const
{
    return core::InvalidIndex != this->indices[queryStart];
}

//------------------------------------------------------------------------------
core::Map<core::String, core::String>
URL::Query() const
{
    if (this->HasQuery())
    {
        core::Map<core::String, core::String> query;
        core::StringBuilder builder;
        builder.Set(this->content.AsCStr(), this->indices[queryStart], this->indices[queryEnd]);
        int kvpStartIndex = 0;
        int kvpEndIndex = 0;
        do
        {
            kvpEndIndex = builder.FindFirstOf(kvpStartIndex, core::EndOfString, "&");
            int keyEndIndex = builder.FindFirstOf(kvpStartIndex, kvpEndIndex, "=");
            if (core::EndOfString != keyEndIndex)
            {
                // key and value
                core::String key(builder.GetSubString(kvpStartIndex, keyEndIndex));
                core::String value(builder.GetSubString(keyEndIndex + 1, kvpEndIndex));
                query.Add(key, value);
            }
            else
            {
                // only key
                core::String key(builder.GetSubString(kvpStartIndex, kvpEndIndex));
                query.Add(key, core::String());
            }
            kvpStartIndex = kvpEndIndex + 1;
        } while (core::EndOfString != kvpEndIndex);
        return query;
    }
    else
    {
        return core::Map<core::String, core::String>();
    }
}

} // namespace ari::io
