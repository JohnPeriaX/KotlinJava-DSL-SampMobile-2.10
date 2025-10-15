plugins {
    id("com.android.application") apply false
    id("com.google.gms.google-services") apply false
    id("com.google.firebase.crashlytics") apply false
}

tasks.register("clean", Delete::class) {
    delete(rootProject.layout.buildDirectory)
}