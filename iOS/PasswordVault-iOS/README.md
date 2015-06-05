# PasswordVault-iOS
Password Vault app for iOS. It is a Nymi Enabled Application. This app was built using Nymi's iOS SDK version 2.0.

There are two active schemes in Xcode that you can choose to run the app and test it with either the Nymulator (simulator for the Nymi band) or with a Nymi band.

### To run the app on the iOS simulator and test it with the Nymulator:
- In Constants.h file, change the IP in `#define NYMULATOR_IP @"10.18.39.1"` to your computer's IP address.
- Select the PasswordVault Nymulator as the active scheme in Xcode.

### To run the app on a iOS device and test it with a Nymi band:
- Select the PasswordVault Device as the active scheme in Xcode.
