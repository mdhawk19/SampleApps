# NymiReferenceApp
This is Android sample app for Nymi SDK 3.0 Beta for Android.

This sample app demonstrates initialization, provisioning, signing messages, and generating random numbers. It is also available in the download package for Nymi SDK 3.0 Beta for Android. The sample app is tested only on Android Studio. However, you can port the project to Eclipse and other IDEs along with the AAR file.

See the Nymi developer [documentation](https://www.nymi.com/dev/beta-documentation/) on more information about how to use the API. 

### Building the app

The Nymi Android API is provided in aar bundle which is a binary distribution of an Android Library Project. You can use the Nymi's Android API's sample app as a template that already has .aar file integrated within the app structure or you can integrate the aar bundle within your app structure yourself. Here's some helpful guidelines if you're integrating the aar file to your app:

1. Open the app in Android Studio.
2. Copy the aar file to the libs folder (create one if it’s not there already) in your project directory. For example, /app/libs/. You could copy both `nymi-api-nymulator.aar` and `nymi-api.aar` files in the libs folder. However, you must specify which aar file you want your app to use depending on whether you’re testing the app with the Nymulator or the Nymi Band. The next step below explains how to specify the aar files in your app.
3. Open build.gradle (module) file and add a depedency to the aar files. Under `dependencies`, add the following line if you intend on running your app against the Nymulator:

    `compile(name:'nymi-api-nymulator', ext:'aar')`

    If you plan on running your app against the Nymi Band, use the following line:

    `compile(name:'nymi-api', ext:'aar')`


4. Open build.gradle (project file, and add a flat filesystem directory as a repository by adding the following under `repositories`:
    
    <code>flatDir {dirs 'libs'}</code>

5. Rebuild your project for changes to take effect.

Once you have the aar file integrated in your app, you can use the methods described in the API reference for initializing, provisioning, and interacting with the Nymi Band.

### Running your app against the Nymulator

When you run the android sample app on a real Android device, it must be able to connect to the same network that the Nymulator can access. 

To set up the IP address of your machine in your application: 

1. Open `MainActivity.java` file.
2. Update `mNymiAdapter.setNymulator(localBuildhostResources());` line to include the IP address where the Nymulator is running.
3. The result should look like: `mNymiAdapter.setNymulator("10.0.1.99");`.

When you are ready to test your Android app, launch the Nymulator. The first time you use the Nymulator you will need to create a simulated Nymi Band by clicking the Add button. The Nymi Band instance will now be able to communicate with your application. To place the Nymi Band instance in provisioning mode, click the button labeled Begin Provisioning.

### Running your app against the Nymi Band

Before you run your app, make sure you have copied `nymi-api.aar` file to /apps/libs/ folder and specified it in `build.gradle` (module) file.

To run your app against the Nymi Band, first place your band in provisioning mode by taping it four times on it. Then, use the sample app running on your Android phone to provision with the band.

### Troubleshooting

#### Error when building sample app: Failed to resolve: :nymi-api-nymulator

* Workaround: Download JDK7 and point Android Studio to the correct location of the JDK.
