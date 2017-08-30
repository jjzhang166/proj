set path=%cd%/../MEmuHyperv;%path%

cls

MEmuManage.exe import MEmu.ova --dry-run

MEmuManage.exe import MEmu.ova

MEmuManage.exe modifyvm MEmu --natpf1 delete "INP"

MEmuManage.exe modifyvm MEmu --natpf1 delete "MVD"

MEmuManage.exe modifyvm MEmu --natpf1 delete "APP"

MEmuManage.exe modifyvm MEmu --natpf1 delete "ADB"

MEmuManage.exe modifyvm MEmu --natpf1 delete "SNS"

MEmuManage.exe modifyvm MEmu --natpf1 "INP,tcp,127.0.0.1,21500,10.0.2.15,21500"

MEmuManage.exe modifyvm MEmu --natpf1 "MVD,tcp,127.0.0.1,21501,10.0.2.15,21501"

MEmuManage.exe modifyvm MEmu --natpf1 "APP,tcp,127.0.0.1,21502,10.0.2.15,21502"

MEmuManage.exe modifyvm MEmu --natpf1 "ADB,tcp,127.0.0.1,21503,10.0.2.15,5555"

MEmuManage.exe modifyvm MEmu --natpf1 "SNS,tcp,127.0.0.1,2l504,10.0.2.15,21504"

MEmuManage.exe modifyvm "MEmu" --"memory" 512

MEmuManage.exe modifyvm "MEmu" --"cpus" 1

MEmuManage.exe sharedfolder add "MEmu" --name "music" --hostpath "C:\Users\Administrator\Music\Â–“£∞≤◊ø“Ù¿÷"
MEmuManage.exe sharedfolder add "MEmu" --name "movie" --hostpath "C:\Users\Administrator\Videos\Â–“£∞≤◊ø ”∆µ"
MEmuManage.exe sharedfolder add "MEmu" --name "picture" --hostpath "C:\Users\Administrator\Pictures\Â–“£∞≤◊ø’’∆¨"
MEmuManage.exe sharedfolder add "MEmu" --name "download" --hostpath "C:\Users\Administrator\Downloads\Â–“£∞≤◊øœ¬‘ÿ"
MEmuManage.exe guestproperty set "MEmu" "imsi" "460000446646798"
MEmuManage.exe guestproperty set "MEmu" "imei" "133524256790010"

MEmuManage.exe guestproperty set "MEmu" "enable_su" "1"

MEmuManage.exe guestproperty set "MEmu" "hardware_opengl" "1"

MEmuManage.exe guestproperty set "MEmu" "host_language" "1000"

MEmuManage.exe guestproperty set "MEmu" "is_customed_resolution" "1"

MEmuManage.exe guestproperty set "MEmu" "is_accl_on" "0"

MEmuManage.exe guestproperty set "MEmu" "microvirt_vm_brand" "iphone"

MEmuManage.exe guestproperty set "MEmu" "microvirt_vm_manufacturer" "iphone"

MEmuManage.exe guestproperty set "MEmu" "microvirt_vm_model" "6 plus"

MEmuManage.exe guestproperty set "MEmu" "resolution_height" "480"

MEmuManage.exe guestproperty set "MEmu" "resolution_width" "800"

MEmuManage.exe guestproperty set "MEmu" "simserial" "89860070200779921015"

MEmuManage.exe guestproperty set "MEmu" "vbox_dpi" "192"

MEmuManage.exe guestproperty set "MEmu" "linenum" "+8617677728760"

MEmuManage.exe guestproperty set "MEmu" "phone_layout" "2"

MEmuManage.exe guestproperty set "MEmu" "vkeyboard_mode" "-3"

pause