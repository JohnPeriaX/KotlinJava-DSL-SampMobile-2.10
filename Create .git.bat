:: 🔹 ลบโฟลเดอร์ .git เดิม (รวมประวัติทั้งหมด)
rmdir /s /q .git

:: 🔹 สร้าง Git repo ใหม่
git init

:: 🔹 เพิ่ม safe.directory (กัน error “detected dubious ownership”)
git config --global --add safe.directory "%cd%"

:: 🔹 ตั้งชื่อและอีเมลสำหรับ commit (เปลี่ยนได้ตามต้องการ)
:: git config user.name "JohnPeriaX"
:: git config user.email "your_email@example.com"

:: 🔹 ตั้ง remote ไปยัง GitHub repo (เปลี่ยน URL ได้ถ้าต้องการ)
git remote add origin https://github.com/JohnPeriaX/KotlinJava-DSL-SampMobile-2.10.git

:: 🔹 เพิ่มไฟล์ทั้งหมดในโปรเจ็กต์
git add .

:: 🔹 commit ครั้งแรก
git commit -m "SAMP MOBILE 2.10 NEW Kotlin DSL"

:: 🔹 เปลี่ยนชื่อ branch จาก main → JohnPeriaX
git branch -M JohnPeriaX

:: 🔹 push ทับ branch เดิมบน GitHub (⚠️ ลบประวัติเดิมทั้งหมด)
git push origin JohnPeriaX --force
