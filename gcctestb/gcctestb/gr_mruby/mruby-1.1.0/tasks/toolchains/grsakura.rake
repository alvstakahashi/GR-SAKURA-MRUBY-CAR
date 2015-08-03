MRuby::Toolchain.new(:grsakura) do |conf|
  toolchain :gcc

  SSP_ROOT ="#{MRUBY_ROOT}/../../"

  conf.cc do |cc|
    cc.command="rx-elf-gcc"
    cc.flags << "-O2 -mcpu=rx600 -mlittle-endian-data -mas100-syntax -mint-register=0 "
    cc.flags << " -DALVSTAKAHASHI"
    cc.include_paths = ["#{MRUBY_ROOT}/include"],
    cc.include_paths << "#{SSP_ROOT}/include"
    cc.include_paths << "#{SSP_ROOT}/ssp"
    cc.include_paths << "#{SSP_ROOT}/arch/RX62N"

    cc.defines = %w(DISABLE_GEMS)
    cc.option_include_path = '-I%s'
    cc.option_define = '-D%s'

    cc.compile_options = "%{flags} -o %{outfile} -c %{infile}"

    conf.linker do |linker|
      linker.command="rx-elf-ld"
    end
    conf.archiver do |archiver|
      archiver.command = "rx-elf-ar"
      archiver.archive_options = 'rcs %{outfile} %{objs}'
    end
  end

  conf.bins = []
end
