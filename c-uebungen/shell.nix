# set pinned nixpkgs with `--arg pinned <path to pin info>
let
  pinnedVersion = pin: builtins.fromJSON (builtins.readFile pin);
  pinnedPkgs = pin:  import (builtins.fetchTarball {
    inherit (pinnedVersion pin) url;
    name = "nixpkgs-${(pinnedVersion pin).date}";
  }) {};
  pkgs' = pinned: (
    if (!isNull pinned) then pinnedPkgs pinned 
    else import <nixpkgs> {});
in
{ pkgs ? pkgs' pinned, pinned ? null }:
with pkgs;
let 


in
mkShell {
  name = "betriebssysteme-c-env";
  buildInputs = with pkgs; [
    # put packages here.    
    gcc
    gnumake
#   gdb
  ];

  shellHook = ''
  '';
}
