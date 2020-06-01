# Drupal 7.x Trustcert module
Module, which allows Drupal to open the SSL connection to remote hosts that have untrusted certificates.
## Description
This module overrides default function "drupal_http_request_function" and sets the SSL context options for a specified list of host and then it calls in turn this function with these options again. The SSL context options are set to the following.
* verify_peer_name = false
* verify_peer = false
* allow_self_signed = true
## Background
If you run Drupal website on HTTPS protocol, you can encounter on status report page in administration section with "drupal_http_request_fail" problem. If your server connectivity to Internet is working well the problem may be in SSL. 
Drupal acts in fact as the web client and test the network connection to itself by calling its pages. The core of this problem is the verification of server (peer) SSL certificate, which is on PHP 5.6 in openssl built-in enabled by default. If the server address is different from the common name field of its certificate or its certificate is self-signed, the server is considered as untrusted and the connection to it is closed.
## Installation
This module is compatible with Drupal 7.
1. Copy module files to <drupal installation>/modules/trustcert
1. Enable module in Drupal modules
1. Manage hosts in Menu: Config -> System -> Trustcert -> Trustcert add host
