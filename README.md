# simple-thesaurus

This is a thesaurus that suggests only the top 1000 most common English words. Based on [a blog post by Morten Just](https://medium.com/@mortenjust/sentenced-to-rewriting-7c8bf45680b5), who thinks that it is a poor idea. (I agree.) Uses [Moby Thesaurus](https://en.wikipedia.org/wiki/Moby_Project) and [Peter Norvig's word lists](https://norvig.com/ngrams/) that are derived from Google Web Trillion Word Corpus. Implemented in [Reason](https://reasonml.github.io/) and styled with [Tachyons](http://tachyons.io/). Built to try out these two technologies.

## Run Project

```sh
npm install
npm start
# in another tab
npm run webpack
```

## Build for Production

```sh
npm run build
npm run webpack:production
```

This will replace the development artifact `build/Index.js` for an optimized version.

**To enable dead code elimination**, change `bsconfig.json`'s `package-specs` `module` from `"commonjs"` to `"es6"`. Then re-run the above 2 commands. This will allow Webpack to remove unused code.