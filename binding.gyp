{
  "targets": [
    {
      "target_name": "accssnode",
      "sources": [
        "src/main.cc",
        "accss/src/argsparser.c",
        "src/error.c",
        "accss/src/ast.c",
        "accss/src/helper.c",
        "accss/src/gettokens.c",
        "accss/src/getpast.c",
        "accss/src/compressor.c",
        "accss/src/translator.c",
        "accss/src/debug.c"
      ]
    },
    {
      "target_name": "action_after_build",
      "type": "none",
      "dependencies": [ "accssnode" ],
      "copies": [
        {
          "files": [ "<(PRODUCT_DIR)/accssnode.node" ],
          "destination": "lib"
        }
      ]
    }
  ]
}