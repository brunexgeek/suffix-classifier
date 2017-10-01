# suffix-classifier  [![Build Status](https://travis-ci.org/brunexgeek/suffix-classifier.svg?branch=master)](https://travis-ci.org/brunexgeek/suffix-classifier)  [![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)

A simple toolkit to generate decision trees for word classification based on minimum suffix match.

## How it works?

To perform the training, we need a corpus composed by words and its classification (correct response). You can have 2 or more classes. Until the end of this secion, we'll use the following corpus to explain the algorithm. The following corpus allow us to classify the gender of people names with the classes 0 (female) and 1 (male).

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

The program ``gentree`` will create a tree with all words in the corpus, in reverse order (i.e. from the root you have 'rasec' instead of 'cesar'). Words with similar suffixes follows the same path in the tree until some letter makes them different. Our corpus will look like this:

<img src="https://raw.githubusercontent.com/brunexgeek/suffix-classifier/explain/images/words.dot.png" width='650px'/>


In the image above, every node (except the root) represents letters in the words of the corpus. Blue nodes are points of decision: at this point we have two or more paths to follow, each of them giving a different response (or classification). These are the most important nodes, because they will define the rules used to build the decision tree.