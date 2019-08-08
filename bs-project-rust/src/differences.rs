use crate::sequence::Sequence;
use std::borrow::Cow;
use std::env;

#[derive(PartialEq, Eq, Debug, Clone)]
pub struct Config {
    pub W_MATCH: isize,
    pub W_MISMATCH: isize,
    pub W_GAP: isize,
    pub MIN_DIFF: usize,
    pub THREADS: usize,
    pub CHUNK_SIZE: usize,
}

impl Default for Config {
    fn default() -> Self {
        Config {
            W_MATCH: 2,
            W_MISMATCH: -1,
            W_GAP: -1,
            MIN_DIFF: 71,
            THREADS: 1,
            CHUNK_SIZE : 50,
        }
    }
}

#[derive(PartialEq, Eq, Debug, Clone)]
pub struct Difference {
    pub index: (usize, usize),
    pub diff: usize,
}

pub trait DiffTable {
    fn to_table(&self) -> String;
}


impl DiffTable for Vec<Difference> {
    fn to_table(&self) -> String {
        let mut table: String = format!("x\ty\tdifference\n");

        let values: Vec<String> = self.iter()
            .map(|diff| format!("{}\t{}\t{}\n", diff.index.0, diff.index.1, diff.diff))
            .collect();

        table.push_str(values.concat().as_str());

        table

    }
    
}


impl<'a> Sequence<'a> {
    /// Calculates the Smith-Waterman distance to another Sequence
    ///
    /// ```
    /// let s1 = Sequence::from_file("<filaA>.fa");
    /// let s2 = Sequence::from_file("<fileB>.fa");
    ///
    /// assert_eq!(s1.get_difference(&s2, &Config::default());
    /// ```
    pub fn get_difference(&self, other: &Sequence, config: &Config) -> usize {
        let mut ancestors_top: Vec<usize> = vec![0; other.len() + 1];
        let mut values: Vec<usize> = vec![0; other.len() + 1];
        let mut max: usize = 0;

        for (i, first_char) in self.items.chars().enumerate() {
            for (j, other_char) in other.items.chars().enumerate() {
                let max_diag: isize = if other.len() - j < self.len() - i {
                    (self.len() - i) as isize
                } else {
                    (other.len() - j) as isize
                };

                 if max as isize + config.W_MATCH * max_diag < config.MIN_DIFF as isize {
                     return 0;
                }

                let matching: isize = ancestors_top[j] as isize
                    + if first_char == other_char {
                        config.W_MATCH
                    } else {
                        config.W_MISMATCH
                    };
                let insertion = values[j] as isize + config.W_GAP;
                let deletion = ancestors_top[j + 1] as isize + config.W_GAP;
                let value = 0.max(insertion).max(deletion).max(matching);
                max = max.max(value as usize);

                values[j + 1] = value as usize;
            }
            ancestors_top = values.clone();
        }
        max
    }

    pub fn compare_to_all_offsets(&self, other: &Sequence, config: &Config) -> Vec<Difference> {
        
        let total_comparisons = other.len() - self.len();

        // dbg!(&self.len());
        // dbg!(&total_comparisons);
        
        let differences: Vec<Difference> = (0..total_comparisons)
            .map(|offset| other.get_subsequence(offset, self.len()))
            .map(|subseq| { 
                // dbg!(&self);
                // dbg!(&subseq);

                let diff = self.get_difference(&subseq, config);
                // dbg!(diff);
                (diff , subseq.index)
                
                })
            .filter(|(diff, _)| *diff >= config.MIN_DIFF)
            .map(|(diff, index)| Difference {index: (self.index, index), diff})
            .collect();

        // dbg!(&differences);
        differences
    }

}






#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn smith_waterman_distance() {
        let s1 = Sequence::from_file("tests/test1.fa");
        let s2 = Sequence::from_file("tests/test2.fa");

        assert_eq!(
            s1.get_difference(
                &s2,
                &Config {
                    MIN_DIFF: 4,
                    ..Default::default()
                }
            ),
            12
        );
    }

}
