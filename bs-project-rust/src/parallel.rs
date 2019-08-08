use std::sync::mpsc::channel;
use std::io::Write;
use std::io;

use crossbeam_utils::thread;

use crate::sequence::Sequence;
use crate::differences::{Config, Difference};

#[derive(Debug, PartialEq, Eq)]
enum Message {
  End,
  Data(Vec<Difference>),
}

pub fn compare_theaded(
  seq_a: &Sequence,
  seq_b: &Sequence,
  config: &Config,
) -> (Vec<Difference>, Vec<Difference>) {
  thread::scope(|scope| {
    let (tx, rx) = channel();

    println!(
      "Creating threads 0..{} ({})",
      config.THREADS - 1,
      config.THREADS
    );
    let mut workers = vec![];

    for slot in 0..config.THREADS {
      let mut size = seq_a.len() / config.THREADS;
      let start = slot * size;

      size += if slot == config.THREADS - 1 {
        seq_a.len() - config.THREADS * size
      } else {
        config.CHUNK_SIZE - 1
      };

      let chunk = seq_a.get_subsequence(start, size);
      let tx = tx.clone();

      workers.push(scope.spawn(move |_| {
        // println!("[Thread {}] started", slot);

        // dbg!(&chunk);

        let n_chunks = chunk.len() - (config.CHUNK_SIZE - 1);

        (0..n_chunks)
          .map(|offset| chunk.get_subsequence(offset, config.CHUNK_SIZE))
          .map(|frame| frame.compare_to_all_offsets(seq_b, &config))
          .filter(|diff_list| !diff_list.is_empty())
          .for_each(|diff_list| {
            if tx.send(Message::Data(diff_list)).is_ok() {
              // println!("[Thread {}] sent diffs", slot);
            } else {
              println!("Could not send data");
            }
          });

        tx.send(Message::End);
        // println!("[Thread {}] done, ending now!", slot);
      }));
    }

    let mut running = config.THREADS;
    let mut full = vec![];
    let mut cleaned = vec![];

    drop(tx);

    println!("(waiting for first match)");
    for (i, message) in rx.iter().enumerate() {
      print!(
        "\r{} ({}/{})",
        ['\\', '|', '/', '-'][i % 4],
        running,
        config.THREADS
      );
      io::stdout().flush().unwrap();
      match message {
        Message::Data(diff_list) => {
          let max_diff = diff_list
            .iter()
            .max_by_key(|diff| diff.diff)
            .expect("Could not find max of list");

          full.append(&mut diff_list.clone());
          cleaned.push(max_diff.to_owned());
        }
        Message::End => running -= 1,
      }
    }
    println!();

    (full, cleaned)
  })
  .expect("Could not create threads")
}
