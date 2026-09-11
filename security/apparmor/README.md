# Hardening xTox with AppArmor

xTox can be confined with AppArmor on Linux to reduce attack vectors in case
remote code execution exploit is being used. Please note that [MAC's] (of
course) does not guarantee perfect security, but it will:
- Deny access to your `~/.bashrc`, `~/.ssh/*`
  `~/.config/path/to/your/password/manager/file`, etc.
- Deny creating autostart entries (in `~/.config/autostart`, etc).
- Deny launching random executables (like `sudo`, `su`, etc...).
- And more.

Consider using additional security measures like [Firejail] to improve security
even more.

Please also note that not all distributions have full AppArmor feature set
available. For example, Debian (at least up to Debian 10 (buster)) does not have
network, DBus mediation available. Also, X Server, shared user configuration
files (like `~/.config/QtProject.conf`, caches, etc), opening web links via
unconfined browsers introduces additional attack vectors, too. So please be
cautious even with number of security measures applied.

**AppArmor profile attaches only to `/usr/bin/xtox` and `/usr/local/bin/xtox`
executables by default**. See [Tuning permissions](#tuning-permissions) for
custom setups.

## Installing profile

Select AppArmor profile from appropriate `security/apparmor/X` subdirectory
depending on what AppArmor version is available for your Linux distribution
release:

- 2.13.3
  - Debian 11 (bullseye) (or newer)
  - Ubuntu 19.10 (or newer)
  - openSUSE Tumbleweed
- 2.13.2
  - Debian 10 (buster)
  - Ubuntu 19.04 (Disco Dingo)
- 2.12.1
  - Debian 9 (stretch) or older
  - Ubuntu 18.10 or older

To enable AppArmor profile on your system, run prepared install script:

```
sudo security/apparmor/x.y.z/install.sh
```
Restart `xTox` if it was already running before enabling AppArmor profile.

## Checking if xTox is actually confined

Run `aa-status` command line utility and check if `xTox` is listed within `X
processes are in enforced mode.` list:
```
sudo aa-status
   ...
21 processes are in enforce mode.
   /usr/lib/ipsec/charon (2421)            
   /usr/sbin/cups-browsed (839)
   ...
   /usr/bin/xtox (16315) xtox
   ...
```

Alternatively, use `ps` and `grep`:

```
ps auxZ | fgrep xtox
xtox (enforce)                  vincas   16315  2.0  1.1 1502292 180220 ?      SLl  12:21   0:38 /usr/bin/xtox
```

If OK it's marked as `(enforce)`. `unconfined` means AppArmor profile is not
attached to the process, no confinement is being applied.

## Troubleshooting

If you believe that some feature is unavailable, or some files you need access
to are inaccessible due to enforced AppArmor profile, check system logs for the
hints.

On Debian/Ubuntu:

```
sudo fgrep DENIED /var/log/syslog
```

On openSUSE, OR if you have `auditd` daemon installed:
```
sudo fgrep DENIED /var/log/audit/audit.log
```

You will see messages like this:
```
type=AVC msg=audit(1549793273.269:149): apparmor="DENIED" operation="open" profile="xtox" name="/home/vincas/.config/klanguageove
rridesrc" pid=3037 comm="xtox" requested_mask="r" denied_mask="r" fsuid=1000 ouid=1000
```

This means that `r`ead access was denied to the file
`/home/vincas/.config/klanguageoverridesrc`, owned by you (ouid 1000), by
AppArmor profile `xtox` (available in `/etc/apparmor.d/usr.bin.xtox`).

Please create issue if you detect new AppArmor `DENIED` messages and you believe
that these denials are relevant for other users too. Meanwhile, workaround by
adding manual rule. DO NOT modify `/etc/apparmor.d/usr.bin.xtox` directly! See
[Tuning permissions](#tuning-permissions) for fixing access issues.

## Tuning permissions

If you need access to files (for file sharing) other than from your `$HOME` or
mounted media, create/modify `/etc/apparmor.d/tunables/usr.bin.xtox.d/local`
file and append writable path variable:

```
@{xtox_additional_rw_dirs} += /path/to/some/directory
```

Alternatively, if you need more custom/advanced rules (not only for file
access), create/modify `/etc/apparmor.d/local/usr.bin.xtox` file.

Rule example for reading only, recursively (note the comma!):

```
/path/to/directory/** r,
```

For reading and writing, recursively:
```
/path/to/directory/** rw,
```

Restart AppArmor to reload profiles after modifications:

```
sudo systemctl restart apparmor
```

If AppArmor restart fails, check syntax errors by invoking AppArmor parser
directly:

```
sudo apparmor_parser -r /etc/apparmor.d/usr.bin.xtox
```

For custom installations, when `xTox` executable is not `/usr/bin/xtox` or
`/usr/local/bin/xtox`:
1. create `/etc/apparmor.d/tunables/usr.bin.xtox.d/local`, adding
   `@{xtox_prefix} += /path/to/your/custom/install/prefix` line.
2. modify `/etc/apparmor.d/usr.bin.xtox` profile attachement path: `profile xtox
   /{usr{,local}/bin/xtox,path/to/your/xtox_executable} {`

Restart AppArmor and [check](#checking-if-xtox-is-actually-confined) if `xTox`
process under custom path is actually confined.

## Other resources

Check [Debian], [Ubuntu], [Upstream AppArmor] Wiki pages for more info.

[Debian]: https://wiki.debian.org/AppArmor
[Firejail]: https://firejail.wordpress.com
[MAC's]: https://en.wikipedia.org/wiki/Mandatory_access_control
[Ubuntu]: https://wiki.ubuntu.com/AppArmor
[Upstream AppArmor]: https://gitlab.com/apparmor/apparmor/wikis/home
