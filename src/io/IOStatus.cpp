#include "IOStatus.hpp"

namespace ari::io
{
    #define _TOSTRING(c) case c: return #c

    //------------------------------------------------------------------------------
    const char*
    IOStatus::ToString(Code c) {
        switch (c) {
            _TOSTRING(Continue);
            _TOSTRING(SwitchingProtocols);
            _TOSTRING(OK);
            _TOSTRING(Created);
            _TOSTRING(Accepted);
            _TOSTRING(NonAuthorativeInformation);
            _TOSTRING(NoContent);
            _TOSTRING(ResetContent);
            _TOSTRING(PartialContent);
            _TOSTRING(MultipleChoices);
            _TOSTRING(MovedPermanently);
            _TOSTRING(Found);
            _TOSTRING(SeeOther);
            _TOSTRING(NotModified);
            _TOSTRING(UseProxy);
            _TOSTRING(TemporaryRedirect);
            _TOSTRING(BadRequest);
            _TOSTRING(Unauthorized);
            _TOSTRING(PaymentRequired);
            _TOSTRING(Forbidden);
            _TOSTRING(NotFound);
            _TOSTRING(MethodNotAllowed);
            _TOSTRING(NotAcceptable);
            _TOSTRING(ProxyAuthenticationRequired);
            _TOSTRING(RequestTimeout);
            _TOSTRING(Conflict);
            _TOSTRING(Gone);
            _TOSTRING(LengthRequired);
            _TOSTRING(PreconditionFailed);
            _TOSTRING(RequestEntityTooLarge);
            _TOSTRING(RequestURITooLarge);
            _TOSTRING(UnsupportedMediaType);
            _TOSTRING(RequestedRangeNotSatisfiable);
            _TOSTRING(ExpectationFailed);
            _TOSTRING(InternalServerError);
            _TOSTRING(NotImplemented);
            _TOSTRING(BadGateway);
            _TOSTRING(ServiceUnavailable);
            _TOSTRING(GatewayTimeout);
            _TOSTRING(HTTPVersionNotSupported);
            _TOSTRING(Cancelled);
            _TOSTRING(DownloadError);
            default: return "InvalidIOStatus";
        }
    }    
    
} // namespace ari::io
