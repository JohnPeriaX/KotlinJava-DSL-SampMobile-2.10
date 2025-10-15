#-dontshrink
#-keepclasseswithmembernames class com.nvidia.devtech.*, com.wardrumstudios.utils.*

-keep class com.nvidia.devtech.* { *; }

-keep class com.wardrumstudios.utils.* { *; }

-keep class com.rstarx.hexrays.game.* { *; }

-dontwarn javax.servlet.**
-dontwarn org.conscrypt.**
-dontwarn org.bouncycastle.**
-dontwarn org.openjsse.**
