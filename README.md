*This project has been created as part of the 42 curriculum by dernst, njooris.*

## Description

Webserv is an HTTP/1.1 server written in C++98, inspired by the NGINX configuration style.  
It handles the GET, POST, and DELETE methods, supports CGI execution (Python, PHP, etc.), file uploads, directory listing, and custom routing via a configuration file.  
The server is fully non-blocking and uses `epoll` for I/O multiplexing.

## Instructions

### Compilation

```bash
make
```

### Execution

```bash
./webserv [configuration_file]
```

If no configuration file is provided, a default configuration is used.

### Configuration file

The configuration file follows an NGINX-inspired syntax. Here is an overview of the available directives:

| Directive | Description |
|---|---|
| `listen` | IP address and port to listen on (e.g. `127.0.0.1:8080`) |
| `client_max_body_size` | Maximum allowed size for client request bodies |
| `error_page` | Custom error pages (e.g. `error_page 404 /errors/404.html`) |
| `location` | Block defining behavior for a specific URI |
| `root` | Internal path that replaces the location prefix in the URI |
| `allow_methods` | Accepted HTTP methods for the route (`GET`, `POST`, `DELETE`) |
| `autoindex` | Enables directory listing when no index file is found (`on`/`off`) |
| `index` | Default file to serve when a directory is requested |
| `return` | HTTP redirection to a new URL |
| `upload_store` | Directory where uploaded files are stored (POST requests) |
| `cgi_path` | Associates a file extension with a CGI interpreter (e.g. `.php /usr/bin/php-cgi`) |

### Example configuration

```nginx
server {
    listen 127.0.0.1:8080;
    client_max_body_size 10M;
    error_page 404 /errors/404.html;

    location / {
        root /var/www/html;
        index index.html;
        allow_methods GET;
        autoindex off;
    }

    location /upload {
        allow_methods POST DELETE;
        upload_store /var/www/uploads;
    }

    location /cgi-bin {
        allow_methods GET POST;
        cgi_path .py /usr/bin/python3;
        cgi_path .php /usr/bin/php-cgi;
    }
}
```

## Features

- NGINX-inspired configuration file
- HTTP/1.1 support
- Methods: GET, POST, DELETE
- CGI execution (Python, PHP, and others)
- File upload
- Directory listing (autoindex)
- HTTP redirections
- Custom error pages
- Multiple ports / virtual servers
- Non-blocking I/O with `epoll`

## Resources

- [RFC 1945 — HTTP/1.0](https://datatracker.ietf.org/doc/html/rfc1945)
- [RFC 3875 — CGI/1.1](https://datatracker.ietf.org/doc/html/rfc3875)
- [MDN — HTTP Overview](https://developer.mozilla.org/en-US/docs/Web/HTTP/Guides/Overview)
- [MDN — HTTP Cookies](https://developer.mozilla.org/en-US/docs/Web/HTTP/Guides/Cookies)
- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/)
- [Linux man — epoll(7)](https://man7.org/linux/man-pages/man7/epoll.7.html)

### AI usage

AI was used during this project to assist with the following tasks:
- Drafting and correcting documentation (this README)
- Clarifying RFC specifications and HTTP concepts
- Suggesting test cases and debugging strategies

All AI-generated content was reviewed, understood, and validated before being included in the project.