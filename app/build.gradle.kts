plugins {
    id("com.android.application")
    id("com.google.gms.google-services")
    id("com.google.firebase.crashlytics")
}

android {
    signingConfigs {
        getByName("debug") {
            storeFile = file("D:\\gtareversed\\keystore_playmarket2.jks")
            storePassword = "hayk2010"
            keyPassword = "hayk2010"
            keyAlias = "key0"
        }
        create("release") {
            storeFile = file("D:\\gtareversed\\keystore_playmarket2.jks")
            keyAlias = "key0"
            storePassword = "hayk2010"
            keyPassword = "hayk2010"
        }
    }
    namespace = "com.rstarx.hexrays"
    //noinspection GradleDependency
    compileSdk = 34

    defaultConfig {
        applicationId = "com.rstarx.hexrays"
        minSdk = 26
        targetSdk = 36
        versionCode = 130
        versionName = "1.0"

        multiDexEnabled = true

        ndk {
            abiFilters.addAll(listOf("armeabi-v7a", "arm64-v8a"))
        }

        testInstrumentationRunner = "androidx.test.runner.AndroidJUnitRunner"
        /*externalNativeBuild {
            cmake {
                cppFlags += "-std=c++11"
            }
        }*/
    }

    buildTypes {
        getByName("debug") {
            isDebuggable = false
            isJniDebuggable = false
            isMinifyEnabled = false
            signingConfig = signingConfigs.getByName("release")

            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
        }

        getByName("release") {
            isDebuggable = false
            isJniDebuggable = false
            isMinifyEnabled = false
            signingConfig = signingConfigs.getByName("release")

            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
        }
    }

    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_11
        targetCompatibility = JavaVersion.VERSION_11
    }

    externalNativeBuild {
        cmake {
            path = file("src/main/cpp/CMakeLists.txt")
            //version = "3.22.1"
        }
    }

    ndkVersion = "26.2.11394342"

    lint {
        abortOnError = false
        checkReleaseBuilds = false
    }

    buildFeatures {
        prefab = true
        viewBinding = true
    }

    packaging {
        jniLibs {
            excludes += "META-INF/*"
        }
        resources {
            excludes += "META-INF/*"
        }
    }
}

dependencies {
    implementation(fileTree(mapOf("dir" to "libs", "include" to listOf("*.jar"))))

    implementation(libs.appcompat)
    implementation(libs.material)
    implementation(libs.constraintlayout)

    implementation(libs.navigation.fragment)
    implementation(libs.navigation.ui)

    testImplementation(libs.junit)
    androidTestImplementation(libs.androidx.test.ext.junit)
    androidTestImplementation(libs.espresso.core)

    implementation(libs.prdownloader)
    implementation(libs.volley)
    implementation(libs.sdp)

    implementation(platform(libs.firebase.bom))
    implementation(libs.firebase.analytics)
    implementation(libs.firebase.crashlytics.ndk)
    implementation(libs.firebase.messaging)

    implementation(libs.ini4j)
    implementation(libs.glide)
    implementation(libs.lifecycle.process)
    implementation(libs.paranoid)
    implementation(libs.shadowhook)
}