"C:\Program Files\7-Zip\7z.exe" a -tzip ".\III.CLEOv2.0.0.0.zip" ".\Release\*"  -xr!*.pdb -xr!*.db -xr!*.exp -xr!*.lib -xr!*VC.*.*
"C:\Program Files\7-Zip\7z.exe" a -tzip ".\VC.CLEOv2.0.0.0.zip" ".\Release\*"  -xr!*.pdb -xr!*.db -xr!*.exp -xr!*.lib -xr!*III.*.*

