# Security Policy

## Supported Versions

Only the latest release of xTox is supported. Security fixes are shipped in a new
release on top of it.

| Version        | Supported          |
| -------------- | ------------------ |
| latest release | :white_check_mark: |
| older releases | :x:                |

## Reporting a Vulnerability

**Please do not report security vulnerabilities through public GitHub issues.**

Instead, report them privately using GitHub's
[Security Advisories](https://github.com/xTox/xTox/security/advisories/new)
feature (repository **Security** tab → *Report a vulnerability*). This keeps the
report confidential until a fix is available.

Please include:

- a description of the issue and its impact,
- the affected version / commit,
- steps to reproduce (proof-of-concept if possible),
- any suggested mitigation.

### What to expect

- We aim to acknowledge a report within **7 days**.
- If the report is accepted, we will work on a fix and coordinate a release and
  public disclosure with you.
- If we disagree with the analysis, we will reply explaining our reasoning.

If the vulnerability is in a dependency of xTox, we will follow that project's
disclosure policy and, where possible, mitigate the issue in xTox meanwhile.

## Scope

Reports about the xTox codebase itself are in scope. Issues that only affect
third-party dependencies, or that require a already-compromised machine, are
generally out of scope but still welcome for coordination with upstream.

