#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Script CGI de test - renvoie une page HTML avec les headers CGI appropriés.
Usage : placez ce fichier dans votre répertoire cgi-bin et rendez-le exécutable.
        chmod +x cgi_test.py
"""

import cgi
import cgitb
import os
import sys
from datetime import datetime

# Active l'affichage des erreurs dans le navigateur (pratique pour le debug)
cgitb.enable()


def get_env_table():
    """Génère un tableau HTML avec les variables d'environnement CGI."""
    cgi_vars = [
        "AUTH_TYPE", "CONTENT_LENGTH", "CONTENT_TYPE",
        "GATEWAY_INTERFACE", "PATH_INFO", "PATH_TRANSLATED",
        "QUERY_STRING", "REMOTE_ADDR", "REMOTE_HOST",
        "REMOTE_IDENT", "REMOTE_USER", "REQUEST_METHOD",
        "SCRIPT_NAME", "SERVER_NAME", "SERVER_PORT",
        "SERVER_PROTOCOL", "SERVER_SOFTWARE",
        "HTTP_HOST", "HTTP_USER_AGENT", "HTTP_ACCEPT",
        "HTTP_ACCEPT_LANGUAGE", "HTTP_REFERER",
    ]

    rows = ""
    for var in cgi_vars:
        value = os.environ.get(var, "<em>non défini</em>")
        rows += f"""
        <tr>
            <td class="var-name">{var}</td>
            <td class="var-value">{value}</td>
        </tr>"""
    return rows


def get_form_data():
    """Récupère et formate les données du formulaire GET/POST."""
    form = cgi.FieldStorage()
    if not form:
        return "<p class='no-data'>Aucune donnée de formulaire reçue.</p>"

    rows = ""
    for key in form.keys():
        value = form.getvalue(key, "")
        rows += f"""
        <tr>
            <td class="var-name">{cgi.escape(key) if hasattr(cgi, 'escape') else key}</td>
            <td class="var-value">{cgi.escape(str(value)) if hasattr(cgi, 'escape') else str(value)}</td>
        </tr>"""

    return f"""
    <table class="data-table">
        <thead><tr><th>Champ</th><th>Valeur</th></tr></thead>
        <tbody>{rows}</tbody>
    </table>"""


def main():
    now = datetime.now().strftime("%d/%m/%Y à %H:%M:%S")
    method = os.environ.get("REQUEST_METHOD", "GET")
    env_table = get_env_table()
    form_data = get_form_data()

    # ── Headers CGI obligatoires ────────────────────────────────────────────
    print("Content-Type: text/html; charset=utf-8")
    print("X-CGI-Test: OK")
    print("X-Generated-By: cgi_test.py")
    print()  # Ligne vide séparant les headers du corps

    # ── Corps HTML ──────────────────────────────────────────────────────────
    html = f"""<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>✅ CGI Test — Réponse OK</title>
    <style>
        :root {{
            --bg: #0d1117;
            --surface: #161b22;
            --border: #30363d;
            --accent: #58a6ff;
            --green: #3fb950;
            --orange: #f0883e;
            --text: #e6edf3;
            --muted: #8b949e;
            --font-mono: 'Courier New', Courier, monospace;
        }}

        * {{ box-sizing: border-box; margin: 0; padding: 0; }}

        body {{
            background: var(--bg);
            color: var(--text);
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', sans-serif;
            min-height: 100vh;
            padding: 2rem 1rem;
        }}

        header {{
            max-width: 900px;
            margin: 0 auto 2rem;
            border-bottom: 1px solid var(--border);
            padding-bottom: 1.5rem;
        }}

        .badge {{
            display: inline-block;
            background: var(--green);
            color: #000;
            font-weight: 700;
            font-size: .75rem;
            padding: .25rem .6rem;
            border-radius: 2rem;
            letter-spacing: .05em;
            margin-bottom: .75rem;
        }}

        h1 {{
            font-size: 1.8rem;
            font-weight: 700;
            color: var(--text);
        }}

        .meta {{
            color: var(--muted);
            font-size: .85rem;
            margin-top: .4rem;
        }}

        .meta span {{
            color: var(--accent);
            font-family: var(--font-mono);
        }}

        main {{
            max-width: 900px;
            margin: 0 auto;
            display: grid;
            gap: 1.5rem;
        }}

        section {{
            background: var(--surface);
            border: 1px solid var(--border);
            border-radius: 8px;
            overflow: hidden;
        }}

        .section-header {{
            display: flex;
            align-items: center;
            gap: .6rem;
            padding: .9rem 1.2rem;
            background: rgba(88,166,255,.06);
            border-bottom: 1px solid var(--border);
            font-size: .9rem;
            font-weight: 600;
        }}

        .icon {{ font-size: 1rem; }}

        .data-table {{
            width: 100%;
            border-collapse: collapse;
            font-size: .85rem;
        }}

        .data-table thead tr {{
            background: rgba(255,255,255,.03);
        }}

        .data-table th {{
            padding: .6rem 1.2rem;
            text-align: left;
            font-weight: 600;
            color: var(--muted);
            font-size: .75rem;
            letter-spacing: .06em;
            text-transform: uppercase;
        }}

        .data-table tr:not(:last-child) td {{
            border-bottom: 1px solid var(--border);
        }}

        .data-table tr:hover td {{
            background: rgba(88,166,255,.04);
        }}

        .var-name {{
            padding: .55rem 1.2rem;
            font-family: var(--font-mono);
            color: var(--orange);
            width: 35%;
            white-space: nowrap;
        }}

        .var-value {{
            padding: .55rem 1.2rem;
            font-family: var(--font-mono);
            color: var(--accent);
            word-break: break-all;
        }}

        .no-data {{
            padding: 1.2rem;
            color: var(--muted);
            font-style: italic;
        }}

        /* Formulaire de test */
        .form-grid {{
            padding: 1.2rem;
            display: grid;
            gap: 1rem;
        }}

        .form-row {{
            display: flex;
            gap: .75rem;
            flex-wrap: wrap;
        }}

        input[type="text"], input[type="email"] {{
            background: var(--bg);
            border: 1px solid var(--border);
            border-radius: 6px;
            color: var(--text);
            padding: .5rem .8rem;
            font-size: .85rem;
            flex: 1 1 180px;
            outline: none;
            transition: border-color .2s;
        }}

        input[type="text"]:focus, input[type="email"]:focus {{
            border-color: var(--accent);
        }}

        label {{
            font-size: .8rem;
            color: var(--muted);
            display: block;
            margin-bottom: .3rem;
        }}

        .field {{ flex: 1 1 180px; }}

        .btn-group {{
            display: flex;
            gap: .75rem;
            flex-wrap: wrap;
            padding: 0 1.2rem 1.2rem;
        }}

        button {{
            padding: .5rem 1.2rem;
            border-radius: 6px;
            border: none;
            font-weight: 600;
            font-size: .85rem;
            cursor: pointer;
            transition: opacity .2s;
        }}

        button:hover {{ opacity: .8; }}

        .btn-get  {{ background: var(--accent); color: #000; }}
        .btn-post {{ background: var(--green);  color: #000; }}

        footer {{
            max-width: 900px;
            margin: 2.5rem auto 0;
            text-align: center;
            color: var(--muted);
            font-size: .8rem;
        }}
    </style>
</head>
<body>

<header>
    <div class="badge">✅ CGI ACTIF</div>
    <h1>Réponse CGI — Test Script</h1>
    <p class="meta">
        Généré le <span>{now}</span> &nbsp;|&nbsp;
        Méthode <span>{method}</span> &nbsp;|&nbsp;
        Script <span>{os.environ.get("SCRIPT_NAME", "cgi_test.py")}</span>
    </p>
</header>

<main>

    <!-- Variables d'environnement CGI -->
    <section>
        <div class="section-header">
            <span class="icon">🌐</span>
            Variables d'environnement CGI
        </div>
        <table class="data-table">
            <thead>
                <tr><th>Variable</th><th>Valeur</th></tr>
            </thead>
            <tbody>
                {env_table}
            </tbody>
        </table>
    </section>

    <!-- Données de formulaire reçues -->
    <section>
        <div class="section-header">
            <span class="icon">📨</span>
            Données reçues ({method})
        </div>
        {form_data}
    </section>

    <!-- Formulaire de test interactif -->
    <section>
        <div class="section-header">
            <span class="icon">🧪</span>
            Tester l'envoi de données
        </div>

        <div class="form-grid" id="form-fields">
            <div class="form-row">
                <div class="field">
                    <label for="nom">Nom</label>
                    <input type="text" id="nom" name="nom" placeholder="Dupont">
                </div>
                <div class="field">
                    <label for="email">Email</label>
                    <input type="email" id="email" name="email" placeholder="test@example.com">
                </div>
                <div class="field">
                    <label for="message">Message</label>
                    <input type="text" id="message" name="message" placeholder="Hello CGI !">
                </div>
            </div>
        </div>

        <div class="btn-group">
            <button class="btn-get" onclick="submitGet()">Envoyer en GET</button>
            <button class="btn-post" onclick="submitPost()">Envoyer en POST</button>
        </div>
    </section>

</main>

<footer>
    <p>cgi_test.py &mdash; Python {sys.version.split()[0]} &mdash; Interface CGI de test</p>
</footer>

<script>
    function getFields() {{
        return {{
            nom:     document.getElementById('nom').value,
            email:   document.getElementById('email').value,
            message: document.getElementById('message').value,
        }};
    }}

    function submitGet() {{
        const f = getFields();
        const qs = new URLSearchParams(f).toString();
        window.location.href = window.location.pathname + '?' + qs;
    }}

    function submitPost() {{
        const f = getFields();
        const form = document.createElement('form');
        form.method = 'POST';
        form.action = window.location.pathname;
        Object.entries(f).forEach(([k, v]) => {{
            const input = document.createElement('input');
            input.type  = 'hidden';
            input.name  = k;
            input.value = v;
            form.appendChild(input);
        }});
        document.body.appendChild(form);
        form.submit();
    }}
</script>

</body>
</html>"""

    print(html)


if __name__ == "__main__":
    main()