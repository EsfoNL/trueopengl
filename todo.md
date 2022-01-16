# To implement
- [ ] class which holds all renderdata
- [ ] class which holds all physicsdata
- [ ] migrate new/delete in vertexrenderbuffer to malloc/dealloc/realloc

## RenderData class todo
- [ ] constructors
  - [ ] layerarray + size
  - [ ] empty
- [ ] add functions
  - [ ] layer + pos
  - [ ] layerarray + size + pos
- [ ] delete functions
  - [ ] pos

### RenderData class info
- public members
  - layerarray
  - mutex
  - maxfps
- private members
  - layercount