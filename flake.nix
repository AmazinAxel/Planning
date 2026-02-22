{
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-25.11";
  outputs = { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs {
        inherit system;
        config.android_sdk.accept_license = true;
      };

      androidComposition = pkgs.androidenv.composeAndroidPackages {
        platformVersions = [ "34" "35" ];
        buildToolsVersions = [ "34.0.0" "35.0.0" ];
        includeNDK = true;
        ndkVersions = [ "27.0.12077973" ];
        includeEmulator = true;
        includeSystemImages = true;
        systemImageTypes = [ "google_apis" ];
        abiVersions = [ "x86_64" "arm64-v8a" ];
        cmakeVersions = [ ];
      };

      androidSdk = androidComposition.androidsdk;
      androidNdk = "${androidSdk}/libexec/android-sdk/ndk/${builtins.head androidComposition.ndk-bundle}";

      perlWithPackages = pkgs.perl.withPackages (ps: with ps; [
        Glib
        GlibObjectIntrospection
        XMLLibXML
        XMLLibXSLT
        XMLParser
        IPCRun
        JSON
        ArchiveTar
        SetScalar
        FileWhich
      ]);
    in {
      devShells.${system}.default = pkgs.mkShell {
        buildInputs = with pkgs; [
          gtkmm4
          nlohmann_json
          gcc
          pkg-config
          meson
          ninja
          # Pixiewood dep cross compile
          flex
          bison
          shaderc
          doxygen
          # Pixiewood deps
          perlWithPackages
          appstream
          gobject-introspection
          libxslt
          # Android
          androidSdk

          # dumb emulation stuff because my phone is running a 32bit OS on a 64bit processor (on a modern android version!) for some reason
          androidSdk
          qemu_kvm
          xcb-util-cursor

          # could be needed?
          #graphviz
          #libxslt
          #docbook_xsl
          #glib
          sassc
          python3
        ];

        shellHook = ''
          export ANDROID_SDK_ROOT="${androidSdk}/libexec/android-sdk"
          export ANDROID_NDK_ROOT="$ANDROID_SDK_ROOT/ndk"
          export ANDROID_HOME="$ANDROID_SDK_ROOT"
        '';
      };
    };
}