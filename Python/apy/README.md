run `python go.py` to get started!

go.py is the developer's interface into this repo. It will help you set up the Nymi SDK, which this project depends on. It will also run app.py in an interactive python shell.

app.py is a Python application that can communicate with Nymi bands. It will try to provision a band if it hasn't yet. Once that's done, you can interactively make calls to functions defined in apy.py

apy.py is a Python API for Nymi band communication. It relies on ffi.py, which is the foreign function interface that communicates with the Nymi SDK library. It also communicates with roaming_auth_server.py, which analogizes the setup one would have for roaming auth.
