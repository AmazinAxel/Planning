{
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-25.11";

  outputs = { self, nixpkgs }:
    let
      system = "x86_64-linux"; # Can include other archs if needed
      pkgs = import nixpkgs { inherit system; };
    in {
      packages.${system} = {
        planning = pkgs.stdenv.mkDerivation {
          pname = "planning";
          version = "2.0.0";
          src = self;

          nativeBuildInputs = with pkgs; [ meson ninja pkg-config wrapGAppsHook4 ];
          buildInputs = with pkgs; [ gtkmm4 glibmm nlohmann_json ];
        };
        default = self.packages.${system}.planning;
      };

      # `nix develop`
      devShells.${system}.default = pkgs.mkShell {
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
    };
}
