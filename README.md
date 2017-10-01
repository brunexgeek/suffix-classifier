# suffix-classifier  [![Build Status](https://travis-ci.org/brunexgeek/suffix-classifier.svg?branch=master)](https://travis-ci.org/brunexgeek/suffix-classifier)  [![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)

A simple toolkit to generate decision trees for word classification based on minimum suffix match.

## Corpus

To perform the training, we need a corpus composed by words and its classification (correct response).

- You can have 2 or more classes in your corpus, but only one class per word;
- Words can include *latin1* alphabetic characters (a-z, A-Z and vowels with diacritic). The text **must** be encoded as ISO-8859-1;
- Classes must be positive integral values.

## How it works?

Until the end of this secion, we'll use the following corpus to explain the algorithm. The following corpus enable us to classify the gender of people names with the classes 0 (female) and 1 (male).

```
    Melissa  0
    Alice    0
    Adele    0
    Hayden   0
    Robert   1
    Cesar    1
    Jayden   1
    Tim      1
```
   > Our corpus is pretty small for the sake of simplicity. A real world corpus must have much more samples.

The first step is to create a *word tree*. The program ``gentree`` will create this tree with all words in the corpus, in reverse order (e.g. from the root you have 'rasec' instead of 'cesar'). Words with similar suffixes follows the same path in the tree until some letter makes them different. The word tree of our example corpus will look like this:

<img src="https://raw.githubusercontent.com/brunexgeek/suffix-classifier/explain/images/words.dot.png" width='700px'/>

In the image above, every node (except the root) represents letters in the words of the corpus. Gray node are terminals (the first letter of the word) and also show the the word classification between parenthesis. Blue nodes with multiple child nodes are points of decision: at this point we have two or more paths to follow, each of them giving a different classification. These are the most important nodes, because they will define the rules used to build the *decision tree*.

> Notice that intermediary nodes between decision points and the root are also marked. This simplify the tree transversal because we can visit only the relevant paths.

Using the *word tree* above, the program ``gentree`` can build the *decision tree*. Basically, each decision point in the *word tree* will became a rule in the *decision tree*. Our *decision tree* is presented below. Nodes with gray fill represents the classification at that point.

<img src="https://raw.githubusercontent.com/brunexgeek/suffix-classifier/explain/images/decision.dot.png" width='700px'/>

To predict a classification for a word using the *decision tree*, one need just to reverse the word letters and tranverse the *decision tree* through the rules.