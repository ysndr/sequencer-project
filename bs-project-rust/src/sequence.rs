use std::borrow::Cow;
use std::fmt;
use std::fs;

#[derive(PartialEq, Eq, Debug, Clone)]
pub struct Sequence<'a> {
    pub index: usize,
    pub items: Cow<'a, str>,
    pub name: Cow<'a, str>,
}

impl<'a> Sequence<'a> {
    /// Returns a new Sequence wrapping the given content
    pub fn new(sequence: impl Into<Cow<'a, str>>, name: impl Into<Cow<'a, str>>) -> Self {
        Sequence {
            index: 0,
            items: sequence.into(),
            name: name.into(),
        }
    }
    /// returns a sequnece that is ppart of the current series,
    /// starting at `start`  and containing `length items`+
    /// ```
    /// let seq = Sequence("1234567890");
    /// let sub_seq = seq.get_subsequence(3, 4);
    ///
    /// assert_eq!(seq.len(), 10);
    /// assert_eq!(sub_seq.len(), 4);
    /// assert_eq!(sub_seq, Sequence("4567"));
    /// ```
    pub fn get_subsequence(&'a self, start: usize, length: usize) -> Self {
        Sequence {
            index: self.index + start,
            items: Cow::from(&self.items[start..start + length]),
            name: Cow::from(&self.name[..]),
        }
    }

    /// Reads a sequence from a .fa file and parses it to keep only valid and
    /// non duplicate items (A, C, G, T)
    ///
    /// ```
    /// let seq = Sequence::from_file("tests/test1.fa");
    /// assert_eq!(seq, Sequence::new("ACACCACTA"));
    /// ```
    pub fn from_file(filename: &str) -> Self {
        let mut content = fs::read_to_string(filename)
            .unwrap_or_else(|_| panic!("Could not read file {}", filename));

        let mut data = content.split_off(content.find('\n').unwrap());
        data = data.split_off(1);

        data.retain(|c| "ACGT".contains(c));

        Sequence::new(data, content)
    }

    pub fn len(&self) -> usize {
        self.items.len()
    }
}

impl <'a> fmt::Display for Sequence<'a> {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self.index {
            0 => write!(f, "('{}' [{}])", self.name, self.len()),
            _ => write!(f, "('{}' [{}, {}])", self.name, self.len(), self.index)
        }
    }

}

#[test]
fn test_get_subsequence() {
    let seq = Sequence::new("1234567890", "oh one sequence");
    let sub_seq = seq.get_subsequence(3, 4);

    assert_eq!(seq.len(), 10);
    assert_eq!(sub_seq.len(), 4);
    assert_eq!(
        sub_seq,
        Sequence {
            index: 3,
            ..Sequence::new("4567", "oh one sequence")
        }
    );
}

#[test]
fn from_file() {
    let seq = Sequence::from_file("tests/test1.fa");
    assert_eq!(seq, Sequence::new("ACACACTA", ">test1.fa"));
}
