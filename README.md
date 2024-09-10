# taaml

Master List Network Server for Tribes Aerial Assault (PS2)

This Master List was built directly from network packet logs from the original Master List run by Sierra. We had the foresight to log the packets the night before the official Master List was terminated, and were able to write this server.

It will accept servers listed by PS2 Clients and the Windows based "AADS Dedicated Server" written by then Inevitable developer "Ricochet".

Please create an issue in the issue tracker if you need help.

### Credits

Created / coded by dreamcast tm `<br>` 2010

Additional research / testing by Ben Pekarek

### Info

taaml.exe uses UDP port 15101 (port forwarding may be required)

Run via .bat file with:

```
@echo off
cls
taaml.exe 0.0.0.0
```

taaml provides a static dummy server entry in the list so you know when it's reachable on a PS2 client.

### Testing

Tribes Aerial Assault on the PS2 has a dual LAN and Online mode when it reaches the Network to look for hosted games. Which means, trying to test taaml on an isolated LAN can be challenging. Any hosted game server clients (from a PS2 or AADS) you run on your LAN will automatically show up in the Online view on TribesAA without taaml needing to be reachable.

### License:

```
--------------------------------------------------------------------
: TRIBES AERIAL ASSAULT MASTER LIST AND INCLUDED SOURCE
: Created / coded by dreamcast tm <br> 2010
: Additional research / testing by Ben Bekarek
--------------------------------------------------------------------

file(s): taaml.c

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
```
