// #![feature(test)]
// extern crate test;

use std::env;
use std::fs;

use clap::*;

mod differences;
mod parallel;
mod sequence;

use differences::{Config, DiffTable};
use parallel::compare_theaded;
use sequence::Sequence;

const USAGE: &str = r#"
FA Sequencer
Author: Yannik Sander

USAGE: sequencer <.fa file> <.fa file> <n threads>  
"#;

fn main() {
  let matches = app_from_crate!()
    .arg(
      Arg::with_name("Match Weight")
        .short("m")
        .long("match")
        .value_name("WEIGHT")
        .help("Sets a custom weight for matches")
        .takes_value(true),
    )
    .arg(
      Arg::with_name("Mismatch Weight")
        .short("s")
        .long("mismatch")
        .value_name("WEIGHT")
        .help("Sets a custom weight for non matches")
        .takes_value(true),
    )
    .arg(
      Arg::with_name("Gap Weight")
        .short("g")
        .long("gap")
        .value_name("WEIGHT")
        .help("Sets a custom weight for gaps")
        .takes_value(true),
    )
    .arg(
      Arg::with_name("Chunk Size")
        .short("c")
        .long("chunksize")
        .value_name("SIZE")
        .help("Sets a custom chunk size")
        .takes_value(true),
    )
    .arg(
      Arg::with_name("Validity threshold")
        .short("t")
        .long("threshold")
        .value_name("THRESHOLD")
        .help("Sets a custom validity threshold")
        .takes_value(true),
    )
    .arg(
      Arg::with_name("Output file")
        .short("o")
        .long("out")
        .value_name("FILE")
        .help("Sets the output file")
        .required(true)
        .takes_value(true),
    )
    .arg(
      Arg::with_name("Thread count")
        .short("n")
        .long("threads")
        .value_name("THREADS")
        .help("Sets the number of threads to use")
        .required(true)
        .takes_value(true),
    )
    .arg(
      Arg::with_name("Full output")
        .short("f")
        .long("full")
        .help("Whether to write raw matching results"),
    )
    .arg(
      Arg::with_name("Input file A")
        .value_name("FILE")
        .help("Input file in fa format")
        .required(true),
    )
    .arg(
      Arg::with_name("Input file B")
        .value_name("FILE")
        .help("Input file in fa format")
        .required(true),
    )
    .get_matches();

  let args: Vec<String> = env::args().collect();

  if args.len() < 5 {
    println!("{}", USAGE);
    std::process::exit(1);
  }

  let in_a = matches.value_of("Input file A").unwrap();
  let in_b = matches.value_of("Input file B").unwrap();
  let out = matches.value_of("Output file").unwrap();
  let full = matches.is_present("Full output");
  let mut config = Config {
    ..Default::default()
  };

  if let Some(weight) = matches.value_of("Match Weight") {
    config.W_MATCH = weight.parse().unwrap();
  }
  if let Some(weight) = matches.value_of("Gap Weight") {
    config.W_GAP = weight.parse().unwrap();
  }
  if let Some(weight) = matches.value_of("Mismatch Weight") {
    config.W_MISMATCH = weight.parse().unwrap();
  }
  if let Some(chunks) = matches.value_of("Chunk Size") {
    config.CHUNK_SIZE = chunks.parse().unwrap();
  }
  if let Some(chunks) = matches.value_of("Validity threshold") {
    config.MIN_DIFF = chunks.parse().unwrap();
  }
  if let Some(threads) = matches.value_of("Thread count") {
    config.THREADS = threads.parse().unwrap();
  }
  run(in_a, in_b, out, full, config);
}

fn run(in_a: &str, in_b: &str, out: &str, full_output: bool, config: Config) {
  let mut seq_a = Sequence::from_file(in_a);
  let mut seq_b = Sequence::from_file(in_b);

  if config != Config::default() {
    println!("Custom config: {:?}", config);
  }

  if seq_a.len() < seq_b.len() {
    std::mem::swap(&mut seq_a, &mut seq_b);
  }

  if seq_a.len() / config.THREADS < config.CHUNK_SIZE {
    println!(
      "Can not distribute target sequence {} among {} threads of size >= {}!",
      seq_a, config.THREADS, config.CHUNK_SIZE
    );
    return;
  }

  let (full, cleaned) = compare_theaded(&seq_a, &seq_b, &config);

  if full_output {
    fs::write(out.to_owned(), full.to_table()).expect("Could not write full diff file");
  } else {
    fs::write(out.to_owned(), cleaned.to_table()).expect("Could not write clean diff file");
  }
}

// #[cfg(test)]
// mod tests {
//   use super::*;
//   use test::Bencher;

//   #[bench]
//   fn bench_run(b: &mut Bencher) {
//     b.iter(|| {
//       run(
//         "/vol/lehre/GTechInf/sox3_region.fa",
//         "/vol/lehre/GTechInf/sry.fa",
//         "/dev/null",
//         false,
//         Config {
//           THREADS: 12,
//           ..Default::default()
//         },
//       )
//     });
//   }
// }
