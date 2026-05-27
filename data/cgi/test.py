#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import html
import urllib.parse
from datetime import datetime


def parse_cookies():

    raw = os.environ.get("HTTP_COOKIE", "")
    cookies = {}

    for part in raw.split(";"):

        part = part.strip()

        if "=" in part:

            k, v = part.split("=", 1)

            cookies[k.strip()] = v.strip()

    return cookies


def parse_body():

    length = os.environ.get(
        "CONTENT_LENGTH",
        "0"
    )

    try:
        length = int(length)

    except:
        length = 0

    if length <= 0:
        return ""

    return os.read(
        0,
        length
    ).decode(
        "utf-8",
        errors="replace"
    )


def parse_form(raw_body):

    method = os.environ.get(
        "REQUEST_METHOD",
        "GET"
    )

    data = {}

    if method == "GET":

        source = os.environ.get(
            "QUERY_STRING",
            ""
        )

    else:

        source = raw_body

    parsed = urllib.parse.parse_qs(
        source
    )

    for k, v in parsed.items():

        data[k] = v[0] if v else ""

    return data


def table_rows(data):

    rows = ""

    for k, v in sorted(data.items()):

        rows += f"""
        <tr>
            <td>{html.escape(str(k))}</td>
            <td>{html.escape(str(v))}</td>
        </tr>
        """

    return rows


def env_table():

    data = {}

    allowed = [

        "REQUEST_METHOD",
        "SCRIPT_NAME",

        "QUERY_STRING",

        "CONTENT_TYPE",
        "CONTENT_LENGTH",

        "SERVER_PROTOCOL",
        "SERVER_SOFTWARE",

        "SERVER_NAME",
        "SERVER_PORT",

        "REMOTE_ADDR",

        "GATEWAY_INTERFACE"
    ]

    for key in allowed:

        value = os.environ.get(key)

        data[key] = value if value else "(empty)"

    for key, value in sorted(
        os.environ.items()
    ):

        if key.startswith(
            "HTTP_"
        ):

            data[key] = value

    return table_rows(data)


def raw_query():

    query = os.environ.get(
        "QUERY_STRING",
        ""
    )

    if not query:

        return """
        <div class="empty">
        no query string
        </div>
        """

    return f"""
    <table>

    <tr>
        <td>QUERY</td>
        <td>{html.escape(query)}</td>
    </tr>

    </table>
    """


def raw_body_table(body):

    if not body:

        return """
        <div class="empty">
        no body
        </div>
        """

    return f"""
    <table>

    <tr>
        <td>BODY</td>
        <td>{html.escape(body)}</td>
    </tr>

    </table>
    """


def form_table(data):

    if not data:

        return """
        <div class="empty">
        no data received
        </div>
        """

    return f"""
    <table>
        {table_rows(data)}
    </table>
    """


def main():

    body = parse_body()

    form = parse_form(body)

    cookies = parse_cookies()

    session_id = cookies.get(
        "id",
        ""
    )

    if session_id.isdigit():
        session_id = str(int(session_id) + 1)

    method = os.environ.get(
        "REQUEST_METHOD",
        "GET"
    )

    now = datetime.now().strftime(
        "%d/%m/%Y · %H:%M:%S"
    )

    cookie_html = ""

    if session_id:

        cookie_html = f"""
        <div class="session-pill">

        <span>🍪</span>

        <span class="sid">
        {html.escape(session_id)}
        </span>

        </div>
        """

    print(
        "Content-Type: text/html; charset=utf-8"
    )

    print()

    print(f"""
<!DOCTYPE html>

<html lang="fr">

<head>

<meta charset="UTF-8">

<meta
name="viewport"
content="width=device-width, initial-scale=1.0">

<title>cgi debug</title>

<link
href="https://fonts.googleapis.com/css2?family=Geist:wght@300;400;500&display=swap"
rel="stylesheet">

<style>

* {{
margin:0;
padding:0;
box-sizing:border-box;
}}

body {{

background:#fff;
color:#111;

font-family:'Geist',sans-serif;

min-height:100vh;

padding:80px 24px 40px;

display:flex;
flex-direction:column;

align-items:center;

gap:28px;
}}

.session-pill {{

position:fixed;

top:20px;
right:24px;

display:flex;
gap:7px;

font-size:.72rem;

color:#888;

border:1px solid #e5e5e5;

border-radius:999px;

padding:5px 12px;
}}

.back {{

position:fixed;

top:20px;
left:24px;

font-size:.78rem;

color:#888;

text-decoration:none;

border:1px solid #e5e5e5;

border-radius:999px;

padding:5px 14px;
}}

h1 {{

font-size:2.3rem;

font-weight:500;
}}

.meta {{

display:flex;

gap:10px;
}}

.badge {{

border:1px solid #e5e5e5;

border-radius:999px;

padding:6px 14px;

font-size:.74rem;

color:#888;
}}

.section {{

width:100%;
max-width:760px;
}}

.label {{

font-size:.68rem;

color:#aaa;

margin-bottom:10px;
}}

table {{

width:100%;

border-collapse:collapse;

font-size:.78rem;
}}

tr:nth-child(even) {{

background:#fafafa;
}}

td {{

padding:10px;

border-bottom:1px solid #eee;
}}

td:first-child {{

width:35%;

color:#888;
}}

.empty {{

border:1px solid #eee;

padding:15px;

border-radius:10px;

color:#999;
}}

.form-box {{

display:flex;
flex-direction:column;
gap:12px;
}}

.form-row {{

display:flex;
gap:12px;
}}

.field {{

flex:1;
}}

input {{

width:100%;

padding:10px;

border-radius:10px;

border:1px solid #e5e5e5;
}}

.buttons {{

display:flex;
gap:10px;
}}

button {{

padding:10px 16px;

border-radius:10px;

border:1px solid #e5e5e5;

background:white;

cursor:pointer;
}}

</style>

</head>

<body>

<a href="/index.html" class="back">

← retour

</a>

{cookie_html}

<h1>
cgi debug
</h1>

<div class="meta">

<div class="badge">

{html.escape(method)}

</div>

<div class="badge">

Python CGI

</div>

<div class="badge">

{html.escape(now)}

</div>

</div>

<div class="section">

<div class="label">
ENVIRONMENT
</div>

<table>

{env_table()}

</table>

</div>

<div class="section">

<div class="label">
QUERY STRING
</div>

{raw_query()}

</div>

<div class="section">

<div class="label">
RAW BODY
</div>

{raw_body_table(body)}

</div>

<div class="section">

<div class="label">
FORM DATA
</div>

{form_table(form)}

</div>

<div class="section">

<div class="label">
TEST REQUEST
</div>

<div class="form-box">

<div class="form-row">

<div class="field">

<input
id="name"
placeholder="john doe">

</div>

<div class="field">

<input
id="email"
placeholder="john@example.com">

</div>

</div>

<div class="field">

<input
id="message"
placeholder="hello webserv">

</div>

<div class="buttons">

<button onclick="sendGet()">

send GET

</button>

<button onclick="sendPost()">

send POST

</button>

</div>

</div>

</div>

<script>

function values() {{

return {{

name:
document.getElementById(
'name'
).value,

email:
document.getElementById(
'email'
).value,

message:
document.getElementById(
'message'
).value

}};
}}

function sendGet() {{

const qs =
new URLSearchParams(
values()
).toString();

window.location=
window.location.pathname
+ '?'
+ qs;
}}

function sendPost() {{

const form =
document.createElement(
'form'
);

form.method='POST';

form.action=
window.location.pathname;

Object.entries(
values()
).forEach(([k,v])=>{{

const input=
document.createElement(
'input'
);

input.type='hidden';

input.name=k;

input.value=v;

form.appendChild(
input
);

}});

document.body.appendChild(
form
);

form.submit();

}}

</script>

</body>

</html>
""")


if __name__ == "__main__":
    main()