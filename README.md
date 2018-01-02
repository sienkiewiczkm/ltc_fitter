# Linearly Transformed Cosines Fitting Tool

This application is part of Master's thesis under title "Area lights in computer games".

## Building

Project should be used with CMake. Refer to CMake documentation. `ltc_fitter` depends on the following libraries 
that need to be installed prior to compilation:

- `boost`, at least 1.63 (used components: `program_options`, `date_time`)

## Usage

Use `.\ltcfitter --help` to see all available commands. Some examples of usage:

```
.\ltc_fitter --help
.\ltc_fitter -b ggx -r 32 -o ggx_32x32.ltc
```

## References

1. Real-Time Polygonal-Light Shading with Linearly Transformed Cosines.
   Eric Heitz, Jonathan Dupuy, Stephen Hill and David Neubelt,
   [Available on the project page](https://eheitzresearch.wordpress.com/415-2/)
2. Understanding the Masking-Shadowing Function in Microfacet-Based BRDFs,
   Eric Heitz,
   INRIA, CNRS, Univ. Grenoble Alpes
3. Microfacet Models for Refraction through Rough Surfaces,
   Walter, Bruce and Marschner, Stephen R. and Li, Hongsong and Torrance, Kenneth E.
4. PBR Diffuse Lighting for GGX+Smith Microsurfaces, Earl Hammon, Jr., GDC, 
   [available here](https://twvideo01.ubm-us.net/o1/vault/gdc2017/Presentations/Hammon_Earl_PBR_Diffuse_Lighting.pdf)