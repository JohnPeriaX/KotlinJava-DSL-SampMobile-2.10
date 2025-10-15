pluginManagement {
    repositories {
        google()
        mavenCentral()
        gradlePluginPortal()
        maven(url = "https://artifacts.applovin.com/android")
    }
    plugins {
        id("com.android.application") version "8.2.1" apply false
        id("com.google.gms.google-services") version "4.4.4" apply false
        id("com.google.firebase.crashlytics") version "3.0.6" apply false
    }
}

dependencyResolutionManagement {
    repositoriesMode.set(RepositoriesMode.FAIL_ON_PROJECT_REPOS)
    repositories {
        google()
        mavenCentral()
        maven(url = "https://jitpack.io")
        maven(url = "https://mint.splunk.com/gradle/")
        maven(url = "https://oss.sonatype.org/content/repositories/snapshots/")
    }
}

rootProject.name = "gtareversed"
include(":app")