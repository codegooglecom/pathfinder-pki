#include <wvfile.h>
#include <wvurl.h>
#include <wvregex.h>
#include <openssl/objects.h>
#include <openssl/x509.h>
#include <assert.h>
#include "util.h"

using namespace boost;

WvX509::DumpMode guess_encoding(WvBuf &buf)
{
    if (buf.used() < 10)
        return WvX509::CertDER;

    if (!strncmp("-----BEGIN", (const char *) buf.peek(0, 10), 10))
        return WvX509::CertPEM;

    return WvX509::CertDER;
}


WvX509::DumpMode guess_encoding(WvStringParm fname)
{
    WvFile f(fname, O_RDONLY);
    WvDynBuf buf;
    size_t read = f.read(buf, 10);

    WvX509::DumpMode mode = guess_encoding(buf);
    if (mode == WvX509::CertPEM)
        return WvX509::CertFilePEM;

    return WvX509::CertFileDER;
}

bool is_md(shared_ptr<WvX509> &x509)
{	
    X509 *cert = x509->get_cert();
    int alg = OBJ_obj2nid(cert->sig_alg->algorithm);
    
    if (alg == NID_md5WithRSAEncryption || alg == NID_md2WithRSAEncryption)
        return true;
      
    return false;
}

#if 0
bool is_valid_host(WvStringParm hostname_or_ip)
{
// If and when WvRegex ever grows more intelligence... do this.
    WvRegex r("^([1-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])(\\."
              "([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])){3}$");
    WvString match;
    r.match(hostname_or_ip, match);
    if (hostname_or_ip == match)
    {
        // This was an IP Address
        return true;
    }
    r.set("^(([a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9\\-]*[a-zA-Z0-9])\\.)*"
          "([A-Za-z]|[A-Za-z][A-Za-z0-9\\-]*[A-Za-z0-9])$");
    r.match(hostname_or_ip, match);
    if (!!match)
        return true;
    else
        return false;
}
#endif

WvUrl rewrite_url(WvUrl url, WvStringParm hostname_or_ip)
{

    // This isn't as generic as I would like, but it's better than nothing.
    WvString newurl;
    newurl.append(url.getproto());
    newurl.append("://");
    newurl.append(hostname_or_ip);
    newurl.append("/");
    newurl.append(url.getfile());

    return WvUrl(newurl);
} 

