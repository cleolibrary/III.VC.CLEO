"C:\Program Files\7-Zip\7z.exe" a -tzip ".\III.CLEOv2.0.0.4.zip" ".\Release\*"  -xr!*.pdb -xr!*.ipdb -xr!*.iobj -xr!*.db -xr!*.exp -xr!*.lib -xr!*VC.*.*
"C:\Program Files\7-Zip\7z.exe" a -tzip  ".\VC.CLEOv2.0.0.4.zip" ".\Release\*"  -xr!*.pdb -xr!*.ipdb -xr!*.iobj -xr!*.db -xr!*.exp -xr!*.lib -xr!*III.*.*

