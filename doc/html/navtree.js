var NAVTREE =
[
  [ "ESAPI-C", "index.html", [
    [ "ESAPI-C User Guide", "index.html", null ],
    [ "Related Pages", "pages.html", [
      [ "Todo List", "todo.html", null ]
    ] ],
    [ "Data Structures", "annotated.html", [
      [ "codec", "structcodec.html", null ],
      [ "element", "structelement.html", null ],
      [ "esapi_ctx", "structesapi__ctx.html", null ],
      [ "Map_t", "struct_map__t.html", null ],
      [ "User_t", "struct_user__t.html", null ],
      [ "UT_array", "struct_u_t__array.html", null ],
      [ "UT_hash_bucket", "struct_u_t__hash__bucket.html", null ],
      [ "UT_hash_handle", "struct_u_t__hash__handle.html", null ],
      [ "UT_hash_table", "struct_u_t__hash__table.html", null ],
      [ "UT_icd", "struct_u_t__icd.html", null ],
      [ "UT_string", "struct_u_t__string.html", null ]
    ] ],
    [ "Data Structure Index", "classes.html", null ],
    [ "File List", "files.html", [
      [ "access_ref.c", "access__ref_8c.html", null ],
      [ "access_ref.h", "access__ref_8h.html", null ],
      [ "authenticator.c", null, null ],
      [ "authenticator.h", null, null ],
      [ "authorization.c", null, null ],
      [ "authorization.h", null, null ],
      [ "base64.c", "base64_8c.html", null ],
      [ "base64.h", "base64_8h.html", null ],
      [ "canonicalize.c", "canonicalize_8c.html", null ],
      [ "canonicalize.h", "canonicalize_8h.html", null ],
      [ "codec.c", "codec_8c.html", null ],
      [ "codec.h", "codec_8h.html", null ],
      [ "crypto.c", "crypto_8c.html", null ],
      [ "crypto.h", "crypto_8h.html", null ],
      [ "esapi.h", "esapi_8h.html", null ],
      [ "executor.c", "executor_8c.html", null ],
      [ "executor.h", "executor_8h.html", null ],
      [ "local_strdup.c", "local__strdup_8c.html", null ],
      [ "log.c", "log_8c.html", null ],
      [ "log.h", "log_8h.html", null ],
      [ "properties.c", "properties_8c.html", null ],
      [ "properties.h", "properties_8h.html", null ],
      [ "README", null, null ],
      [ "sec_context.c", "sec__context_8c.html", null ],
      [ "unix_codec.c", "unix__codec_8c.html", null ],
      [ "unix_codec.h", "unix__codec_8h.html", null ],
      [ "user.c", null, null ],
      [ "user.h", "user_8h.html", null ],
      [ "utarray.h", "utarray_8h.html", null ],
      [ "uthash.h", "uthash_8h.html", null ],
      [ "utlist.h", "utlist_8h.html", null ],
      [ "utstring.h", "utstring_8h.html", null ],
      [ "validate.c", "validate_8c.html", null ],
      [ "validate.h", "validate_8h.html", null ],
      [ "windows_codec.c", "windows__codec_8c.html", null ],
      [ "windows_codec.h", "windows__codec_8h.html", null ]
    ] ],
    [ "Globals", "globals.html", null ]
  ] ]
];

function createIndent(o,domNode,node,level)
{
  if (node.parentNode && node.parentNode.parentNode)
  {
    createIndent(o,domNode,node.parentNode,level+1);
  }
  var imgNode = document.createElement("img");
  if (level==0 && node.childrenData)
  {
    node.plus_img = imgNode;
    node.expandToggle = document.createElement("a");
    node.expandToggle.href = "javascript:void(0)";
    node.expandToggle.onclick = function() 
    {
      if (node.expanded) 
      {
        $(node.getChildrenUL()).slideUp("fast");
        if (node.isLast)
        {
          node.plus_img.src = node.relpath+"ftv2plastnode.png";
        }
        else
        {
          node.plus_img.src = node.relpath+"ftv2pnode.png";
        }
        node.expanded = false;
      } 
      else 
      {
        expandNode(o, node, false);
      }
    }
    node.expandToggle.appendChild(imgNode);
    domNode.appendChild(node.expandToggle);
  }
  else
  {
    domNode.appendChild(imgNode);
  }
  if (level==0)
  {
    if (node.isLast)
    {
      if (node.childrenData)
      {
        imgNode.src = node.relpath+"ftv2plastnode.png";
      }
      else
      {
        imgNode.src = node.relpath+"ftv2lastnode.png";
        domNode.appendChild(imgNode);
      }
    }
    else
    {
      if (node.childrenData)
      {
        imgNode.src = node.relpath+"ftv2pnode.png";
      }
      else
      {
        imgNode.src = node.relpath+"ftv2node.png";
        domNode.appendChild(imgNode);
      }
    }
  }
  else
  {
    if (node.isLast)
    {
      imgNode.src = node.relpath+"ftv2blank.png";
    }
    else
    {
      imgNode.src = node.relpath+"ftv2vertline.png";
    }
  }
  imgNode.border = "0";
}

function newNode(o, po, text, link, childrenData, lastNode)
{
  var node = new Object();
  node.children = Array();
  node.childrenData = childrenData;
  node.depth = po.depth + 1;
  node.relpath = po.relpath;
  node.isLast = lastNode;

  node.li = document.createElement("li");
  po.getChildrenUL().appendChild(node.li);
  node.parentNode = po;

  node.itemDiv = document.createElement("div");
  node.itemDiv.className = "item";

  node.labelSpan = document.createElement("span");
  node.labelSpan.className = "label";

  createIndent(o,node.itemDiv,node,0);
  node.itemDiv.appendChild(node.labelSpan);
  node.li.appendChild(node.itemDiv);

  var a = document.createElement("a");
  node.labelSpan.appendChild(a);
  node.label = document.createTextNode(text);
  a.appendChild(node.label);
  if (link) 
  {
    a.href = node.relpath+link;
  } 
  else 
  {
    if (childrenData != null) 
    {
      a.className = "nolink";
      a.href = "javascript:void(0)";
      a.onclick = node.expandToggle.onclick;
      node.expanded = false;
    }
  }

  node.childrenUL = null;
  node.getChildrenUL = function() 
  {
    if (!node.childrenUL) 
    {
      node.childrenUL = document.createElement("ul");
      node.childrenUL.className = "children_ul";
      node.childrenUL.style.display = "none";
      node.li.appendChild(node.childrenUL);
    }
    return node.childrenUL;
  };

  return node;
}

function showRoot()
{
  var headerHeight = $("#top").height();
  var footerHeight = $("#nav-path").height();
  var windowHeight = $(window).height() - headerHeight - footerHeight;
  navtree.scrollTo('#selected',0,{offset:-windowHeight/2});
}

function expandNode(o, node, imm)
{
  if (node.childrenData && !node.expanded) 
  {
    if (!node.childrenVisited) 
    {
      getNode(o, node);
    }
    if (imm)
    {
      $(node.getChildrenUL()).show();
    } 
    else 
    {
      $(node.getChildrenUL()).slideDown("fast",showRoot);
    }
    if (node.isLast)
    {
      node.plus_img.src = node.relpath+"ftv2mlastnode.png";
    }
    else
    {
      node.plus_img.src = node.relpath+"ftv2mnode.png";
    }
    node.expanded = true;
  }
}

function getNode(o, po)
{
  po.childrenVisited = true;
  var l = po.childrenData.length-1;
  for (var i in po.childrenData) 
  {
    var nodeData = po.childrenData[i];
    po.children[i] = newNode(o, po, nodeData[0], nodeData[1], nodeData[2],
        i==l);
  }
}

function findNavTreePage(url, data)
{
  var nodes = data;
  var result = null;
  for (var i in nodes) 
  {
    var d = nodes[i];
    if (d[1] == url) 
    {
      return new Array(i);
    }
    else if (d[2] != null) // array of children
    {
      result = findNavTreePage(url, d[2]);
      if (result != null) 
      {
        return (new Array(i).concat(result));
      }
    }
  }
  return null;
}

function initNavTree(toroot,relpath)
{
  var o = new Object();
  o.toroot = toroot;
  o.node = new Object();
  o.node.li = document.getElementById("nav-tree-contents");
  o.node.childrenData = NAVTREE;
  o.node.children = new Array();
  o.node.childrenUL = document.createElement("ul");
  o.node.getChildrenUL = function() { return o.node.childrenUL; };
  o.node.li.appendChild(o.node.childrenUL);
  o.node.depth = 0;
  o.node.relpath = relpath;

  getNode(o, o.node);

  o.breadcrumbs = findNavTreePage(toroot, NAVTREE);
  if (o.breadcrumbs == null)
  {
    o.breadcrumbs = findNavTreePage("index.html",NAVTREE);
  }
  if (o.breadcrumbs != null && o.breadcrumbs.length>0)
  {
    var p = o.node;
    for (var i in o.breadcrumbs) 
    {
      var j = o.breadcrumbs[i];
      p = p.children[j];
      expandNode(o,p,true);
    }
    p.itemDiv.className = p.itemDiv.className + " selected";
    p.itemDiv.id = "selected";
    $(window).load(showRoot);
  }
}

