# suffix-classifier  [![Build Status](https://travis-ci.org/brunexgeek/suffix-classifier.svg?branch=master)](https://travis-ci.org/brunexgeek/suffix-classifier)  [![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)

A simple toolkit to generate decision trees for word classification based on minimum suffix match.

## Build

This project requires:

- CMake 2.8 or above
- C++11 compiler (tested with GNU C++ 4.8.4)

Use the following commands to build the programs:

    # mkdir build
    # cd build
    # cmake .. && make

## Runnning

Use the program ``gentree`` to generate the *decision tree* from a corpus:

    # ./gentree ../corpus/names.txt names.tree

Use the program ``gencode`` to generate C source code from a *decision tree*:

    # ./gencode names.tree generated.c

The program ``evaluate`` enable you to test the classifier. Just replace the file ``generated.c`` in the ``source`` directory and recompile the program:

    # cp generated.c ../source/generated.c
    # make evaluate

You can evaluate interactively (providing words through keyboard)

    # evaluate

or using a input labeled file

    # evaluate ../corpus/names.txt
    # evaluate ../corpus/test.txt


## Corpus

To perform the training, we need a corpus composed by words and its classification (correct response).

- You can have 2 or more classes in your corpus, but only one class per word;
- Words can include *latin1* alphabetic characters (a-z, A-Z and vowels with diacritic). The text **must** be encoded as ISO-8859-1;
- Classes must be positive integral values.

## How it works?

The goal of the algorithm it to build a decision tree which uses the smallest suffixes possible to decide the classification of the input word.

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

The first step is to create a *word tree*. The program ``gentree`` will create this tree with all words in the corpus, in reverse order (e.g. from the root you have 'rasec' instead of 'cesar'). Words with similar suffixes follow the same path in the tree until some letter makes them different. To illustrate, consider the following corpus which enable us to classify the gender of people names with the classes 0 (female) and 1 (male).

   > Our corpus is small for the sake of simplicity. A real world corpus must have much more samples.

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

The *word tree* of the above corpus will look like this:

<img src="https://raw.githubusercontent.com/brunexgeek/suffix-classifier/explain/images/words.dot.png" width='700px'/>

In the image above, every node (except the root) represents letters in the words of the corpus. Gray node are terminals (the first letter of the word) and include the word classification between parenthesis. Blue nodes are points of decision or nodes between the root and points of decision. Points of decision are:

* nominal nodes that have child nodes with more than one classification;
* terminal nodes that have different classification than some of its child nodes;

Notice that intermediary nodes between decision points and the root are also marked to simplify the tree transversal (this enable us to visit only the relevant paths).

The next step is to generate the *decision tree*. The program ``gentree`` transverse the *word tree* and create one or more *decision nodes* (decision tree nodes) for each *decision point* in the *word tree*.

Our *decision tree* is presented below. Nodes in gray represents the classification at that point.

<img src="https://raw.githubusercontent.com/brunexgeek/suffix-classifier/explain/images/decision.dot.png" width='700px'/>

To predict a classification for a word using the *decision tree*, one need just to reverse the input word and tranverse the *decision tree* through the decisions.