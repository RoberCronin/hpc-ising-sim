{pkgs, ...}: {
  devenv.warnOnNewVersion = false;
  packages = with pkgs; [
    cmake
    ninja
    stdenv.cc.cc.lib
    gnumake
  ];

  languages.python = {
    enable = true;
    package = pkgs.python311;
    venv = {
      enable = true;
      quiet = true;
      requirements = ''
        ./libising[dev]
        numpy
        matplotlib
      '';
    };
  };

  env = {
    LD_LIBRARY_PATH = "${pkgs.stdenv.cc.cc.lib}/lib";
  };

  scripts = {
    benchmark = {
      exec = "pip install -qe libising && python analysis/benchmark.py";
      description = "Benchmarks the Ising simulation";
    };
  };
}
