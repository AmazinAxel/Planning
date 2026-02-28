{
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-25.11";

  outputs = { self, nixpkgs }:
    let
      systems = [ "x86_64-linux" "aarch64-linux" ];
      allSystems = nixpkgs.lib.genAttrs systems;
    in {
      packages = allSystems (system:
        let pkgs = nixpkgs.legacyPackages.${system}; in {
          planning = pkgs.stdenv.mkDerivation {
            pname = "planning";
            version = "2.0.0";
            src = self;

            nativeBuildInputs = with pkgs; [ meson ninja pkg-config wrapGAppsHook4 ];
            buildInputs = with pkgs; [ gtkmm4 glibmm nlohmann_json ];
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
              ccache # caching
              ninja
            ];
          };
        }
      );
    };
}
