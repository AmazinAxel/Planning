{
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-25.11";

  outputs = { self, nixpkgs }:
    let
      systems = [ "x86_64-linux" "aarch64-linux" ];
      allSystems = nixpkgs.lib.genAttrs systems;
    in {
      packages = allSystems (system:
        let
          makePlanning = { stdenv, buildPkgs, nativePkgs }: stdenv.mkDerivation {
            pname = "planning";
            version = "2.0.0";
            src = self;
            nativeBuildInputs = with nativePkgs; [
              meson ninja pkg-config wrapGAppsHook4
            ];
            buildInputs = with buildPkgs; [
              gtkmm4 glibmm nlohmann_json
            ];
          };
          pkgs = nixpkgs.legacyPackages.${system};

        in {
          planning = makePlanning { # native
            stdenv = pkgs.stdenv;
            buildPkgs = pkgs;
            nativePkgs = pkgs;
          };

          planning-aarch64 = let # `nix build .#planning-aarch64`
            cross = pkgs.pkgsCross.aarch64-multiplatform; # cross comp
            in makePlanning {
              stdenv = cross.stdenv;
              buildPkgs = cross;
              nativePkgs = pkgs;
            };
          default = self.packages.${system}.planning;
        }
      );

      # `nix develop`
      devShells = allSystems (system:
        let pkgs = nixpkgs.legacyPackages.${system}; in {
          default = pkgs.mkShell {
            buildInputs = with pkgs; [
              gtkmm4 # C++ gtk
              nlohmann_json # json lib
              gcc
              pkg-config
              meson
              ninja
            ];
          };
        }
      );
    };
}
