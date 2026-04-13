# AtmosphereLutGenerator
Generates a binary LUT for an atmosphere shader and saves it as a file.

## .atmosphere file data layout:
| Offset (bytes) | Size (bytes) | Type | Name |
|---|---|---|---|
| 0 | 4 | uint32 | magic number (0x41544D4F = 'ATMO') |
| 4 | 4 | uint32 | file format version |
| 8 | 4 | uint32 | transmittance image format |
| 12 | 4 | uint32 | transmittance bytes per pixel |
| 16 | 4 | uint32 | transmittance ray points |
| 20 | 4 | float | transmittance ray points curve |
| 24 | 4 | uint32 | transmittance height points |
| 28 | 4 | float | transmittance height points curve |
| 32 | 4 | uint32 | transmittance samples |
| 36 | 4 | uint32 | inscattering image format |
| 40 | 4 | uint32 | inscattering bytes per pixel |
| 44 | 4 | uint32 | inscattering ray points |
| 48 | 4 | float | inscattering ray points curve |
| 52 | 4 | uint32 | inscattering sun points |
| 56 | 4 | float | inscattering sun points curve |
| 60 | 4 | uint32 | inscattering height points |
| 64 | 4 | float | inscattering height points curve |
| 68 | 4 | uint32 | inscattering samples |
| 72 | 4 | float | atmosphere start |
| 76 | 4 | float | sea level |
| 80 | 4 | float | atmosphere end |
| 84 | 4 | float | rayleigh scale height |
| 88 | 4 | float | mie scale height |
| 92 | 4 | float | mie anisotropy |
| 96 | 4 | float | rayleigh scattering red |
| 100 | 4 | float | rayleigh scattering green |
| 104 | 4 | float | rayleigh scattering blu |
| 108 | 4 | float | mie scattering red |
| 112 | 4 | float | mie scattering green |
| 116 | 4 | float | mie scattering blue |
| 120 | 4 | float | mie absorption red |
| 124 | 4 | float | mie absorption green |
| 128 | 4 | float | mie absorption blue |
| 132 | padding | padding | reserved |
| 256 | transmittance bytes per pixel * transmittance ray points * transmittance height points | tightly packed pixel data | transmittance |
| transmittance bytes per pixel * transmittance ray points * transmittance height points | inscattering bytes per pixel * inscattering ray points * inscattering sun points * inscattering height points | tightly packed pixel data | inscattering |
