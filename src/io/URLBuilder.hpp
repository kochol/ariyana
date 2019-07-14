#pragma once

#include "URL.hpp"
#include "core/string/StringBuilder.hpp"

namespace ari::io
{
    /**
        @class ari::io::URLBuilder
        @ingroup IO
        @brief build and manipulate URLs
            
        This allows to build or rebuild a URL from its parts. Constructing
        from an URL or calling the ParseURL() method will parse the URL parts
        into the URLBuilder. After that call the various setter methods
        to manipulate the URL parts, and call Build() to build a new 
        URL from them. Alternatively, build a URL from scratch by setting
        all required URL parts on an default-constructed URLBuilder
        and create the URL by calling the Build() method
    
        @see URL
    */
    class URLBuilder {
    public:
        /// default constructor
        URLBuilder();
        /// construct from URL
        URLBuilder(const URL& url);

        /// setup parts from URL
        void ParseURL(const URL& url);
        /// build URL from parts
        URL BuildURL();
        
        /// the scheme URL part (e.g. "http")
        core::String Scheme;
        /// the URL user part
        core::String User;
        /// the URL password part (clear-text!)
        core::String Password;
        /// the URL host-address part
        core::String Host;
        /// the port-number part
        core::String Port;
        /// local-path part
        core::String Path;
        /// the query part as key/value map
        core::Map<core::String, core::String> Query;
        /// the fragment part
        core::String Fragment;
        
    private:
        core::StringBuilder stringBuilder;
    };

} // namespace ari::io
