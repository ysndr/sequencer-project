{pkgs ? import (if pin == false then <nixpkgs> else pin) {},
 pin ? ./nixpkgs.nix, ... }:
let
  moz_overlay = import (builtins.fetchTarball https://github.com/mozilla/nixpkgs-mozilla/archive/master.tar.gz);
  nixpkgs = import pkgs.path { overlays = [ moz_overlay ]; };
in with nixpkgs;
let
  nur = (import (builtins.fetchTarball "https://github.com/nix-community/NUR/archive/master.tar.gz") {
      pkgs=pkgs;
  }).repos;
  # ------------ Commands ----------------

  # ------------------Rust-------------------
  ruststable = latest.rustChannels.stable.rust.override {
    extensions = [ "rust-src" "rls-preview" "rust-analysis" "rustfmt-preview" ];
  };

  compiler-pkgs = [
    openssl pkgconfig nasm ruststable cmake zlib
    lldb
    # rustup
    # racer
    rustracer
    gcc
    cmake
    gnumake
    zlib
    # gcc-arm-embedded
    binutils.bintools
  ] ++  lib.optional stdenv.isDarwin darwin.apple_sdk.frameworks.Security;


  shell = mkShell rec {
    name = "sequencer-env";
    buildInputs = [ ] ++ compiler-pkgs;
  };

in {
  inherit pkgs nur python-env shell;
}
