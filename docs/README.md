# VertexNova Events Documentation

Documentation for the VertexNova Events library.

## Generating API Documentation

Generate API documentation with Doxygen:

```bash
cmake -DENABLE_DOXYGEN=ON -B build
cmake --build build --target doc_doxygen
```

Documentation will be available at `build/docs/html/index.html`.

## Requirements

- Doxygen 1.8.13 or higher
- Graphviz (for class diagrams)
