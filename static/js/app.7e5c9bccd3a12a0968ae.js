webpackJsonp([2,0],[function(t,e,n){"use strict";function o(t){return t&&t.__esModule?t:{default:t}}var s=n(3),a=o(s),r=n(91),c=o(r),u=n(58),l=o(u);n(72);var i=n(79),d=o(i),f=n(92),p=o(f),h=n(83),_=o(h),v=n(81),m=o(v),b=n(80),x=o(b),y=n(82),w=o(y);a.default.use(l.default),a.default.use(c.default),a.default.use(p.default);var C=new p.default({routes:[{path:"/",component:_.default},{path:"/home",component:_.default},{path:"/doc",component:m.default},{path:"/doc/:theme",component:m.default},{path:"/doc/:theme/:item",component:m.default},{path:"/api",component:x.default},{path:"/download",component:w.default}]});new a.default;new a.default({el:"#app",template:"<App/>",render:function(t){return t(d.default)},components:{App:d.default},router:C})},,,,,,,function(t,e,n){"use strict";function o(t){return t&&t.__esModule?t:{default:t}}Object.defineProperty(e,"__esModule",{value:!0}),e.bus=void 0;var s=n(3),a=o(s);e.bus=new a.default},,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,function(t,e){"use strict";Object.defineProperty(e,"__esModule",{value:!0}),e.default=[]},function(t,e){"use strict";function n(t){var e=0,n=0;if(t.offsetParent)for(e=t.offsetLeft,n=t.offsetTop,t=t.offsetParent;t;)e+=t.offsetLeft,n+=t.offsetTop,t=t.offsetParent;return{x:e,y:n}}function o(t){var e=document.getElementById(t);if(e){var o=n(e).y;o=o-document.body.scrollTop-60,window.scrollBy(0,o)}}Object.defineProperty(e,"__esModule",{value:!0}),e.ScrollToControl=o},function(t,e,n){"use strict";function o(t){return t&&t.__esModule?t:{default:t}}Object.defineProperty(e,"__esModule",{value:!0});var s=n(84),a=o(s);n(7);e.default={name:"app",components:{"top-nav":a.default}}},function(t,e,n){"use strict";Object.defineProperty(e,"__esModule",{value:!0});n(7);e.default={}},function(t,e,n){"use strict";function o(t){return t&&t.__esModule?t:{default:t}}Object.defineProperty(e,"__esModule",{value:!0});var s=(n(7),n(46)),a=n(45),r=o(a),c=n(78),u=o(c);e.default={data:function(){return{doc_nav:"",content:""}},methods:{scroll:function(t){t?(0,s.ScrollToControl)(t):window.scrollBy(0,-document.body.scrollTop)},fetch_doc_nav:function(){var t=this,e="static/docs/nav.json";this.$http.get(e).then(function(e){t.doc_nav=e.body},function(t){})},fetch_doc_content:function(){var t=this,e=this.$route.params,n=e.theme?e.theme:"install",o=e.item?e.item:"";if(r.default.theme&&n===r.default.theme)this.scroll(o);else if(n in r.default)this.content=r.default[n],r.default.theme=n,this.scroll(o);else{var s="static/docs/"+n+".md";this.$http.get(s).then(function(e){t.content=(0,u.default)(e.body),r.default[n]=t.content,t.scroll(o)},function(t){}),r.default.theme=n}}},created:function(){this.fetch_doc_nav(),this.fetch_doc_content()},watch:{$route:function(t,e){this.fetch_doc_content()}},destroyed:function(){r.default.theme=""}}},function(t,e,n){"use strict";Object.defineProperty(e,"__esModule",{value:!0});n(7);e.default={}},function(t,e,n){"use strict";Object.defineProperty(e,"__esModule",{value:!0});n(7);e.default={}},function(t,e){"use strict";Object.defineProperty(e,"__esModule",{value:!0}),e.default={name:"nav",data:function(){return{top:[]}},created:function(){this.fetch_data()},methods:{fetch_data:function(){var t=this;this.$http.get("static/top.json").then(function(e){t.top=e.body},function(t){})},handleSelect:function(t,e){}}}},,,,,,,,,,,,,,,,,,,,function(t,e){},function(t,e){},function(t,e){},function(t,e){},function(t,e){},function(t,e){},,function(t,e,n){n(76);var o=n(6)(n(47),n(89),null,null);t.exports=o.exports},function(t,e,n){var o=n(6)(n(48),n(88),null,null);t.exports=o.exports},function(t,e,n){n(73);var o=n(6)(n(49),n(85),null,null);t.exports=o.exports},function(t,e,n){n(77);var o=n(6)(n(50),n(90),null,null);t.exports=o.exports},function(t,e,n){n(75);var o=n(6)(n(51),n(87),null,null);t.exports=o.exports},function(t,e,n){n(74);var o=n(6)(n(52),n(86),"data-v-17b44d5a",null);t.exports=o.exports},function(t,e){t.exports={render:function(){var t=this,e=t.$createElement,n=t._self._c||e;return n("el-row",{staticClass:"doc"},[n("el-col",{staticClass:"sidebar",attrs:{span:6}},[n("div",{staticClass:"doc-nav"},[n("h2",[t._v("Documentation")]),t._v(" "),n("p",[n("a",{attrs:{href:"https://github.com/hust220/nsp"}},[t._v("Github")])]),t._v(" "),n("ul",t._l(t.doc_nav,function(e){return n("li",[n("strong",[n("a",{class:[e.href=="#"+t.$route.path?"active":""],attrs:{href:e.href},domProps:{textContent:t._s(e.text)}})]),t._v(" "),n("ul",t._l(e.items,function(e){return n("li",[n("a",{class:[e.href=="#"+t.$route.path?"active":""],attrs:{href:e.href},domProps:{textContent:t._s(e.text)}})])}))])}))])]),t._v(" "),n("el-col",{staticClass:"main",attrs:{span:18,offset:6}},[n("div",{staticClass:"doc-content",domProps:{innerHTML:t._s(t.content)}})])],1)},staticRenderFns:[]}},function(t,e){t.exports={render:function(){var t=this,e=t.$createElement,n=t._self._c||e;return n("el-row",{staticClass:"top-nav"},[n("el-menu",{staticClass:"el-menu-demo",attrs:{"default-active":"1",mode:"horizontal"},on:{select:t.handleSelect}},[n("el-col",{attrs:{span:22,offset:1}},[n("el-menu-item",{attrs:{index:"1"}},[n("img",{staticClass:"nsp-logo",attrs:{src:"static/"+t.top.logo}})]),t._v(" "),n("el-menu-item",{attrs:{index:"1"}},[n("router-link",{attrs:{to:"/home"}},[t._v("Home")])],1),t._v(" "),n("el-menu-item",{attrs:{index:"1"}},[n("router-link",{attrs:{to:"/doc"}},[t._v("Documentation")])],1),t._v(" "),n("el-menu-item",{attrs:{index:"1"}},[n("router-link",{attrs:{to:"/download"}},[t._v("Download")])],1),t._v(" "),n("el-menu-item",{attrs:{index:"1"}},[n("el-menu-item",{attrs:{index:"3"}},[n("a",{attrs:{href:"mailto:wj_hust08@hust.edu.cn?cc=yxiao@hust.edu.cn"}},[t._v("Contact")])])],1)],1)],1),t._v(" "),n("div",{staticClass:"line"})],1)},staticRenderFns:[]}},function(t,e){t.exports={render:function(){var t=this,e=t.$createElement;t._self._c||e;return t._m(0)},staticRenderFns:[function(){var t=this,e=t.$createElement,n=t._self._c||e;return n("div",{staticClass:"home"},[n("table",[n("tr",[n("td",{attrs:{rowspan:"2"}},[n("img",{attrs:{src:"static/logo.png"}})]),t._v(" "),n("td",{staticClass:"home-nsp-intro",attrs:{colspan:"2"}},[t._v("Nucleic acids"),n("br"),t._v("Structure Prediction")])]),t._v(" "),n("tr",[n("td",[n("a",{attrs:{href:"#/doc"}},[t._v("Documentation")])]),t._v(" "),n("td",[n("a",{attrs:{href:"https://github.com/hust220/nsp"}},[t._v("Github")])])])])])}]}},function(t,e){t.exports={render:function(){var t=this,e=t.$createElement,n=t._self._c||e;return n("div",{staticClass:"api"},[n("el-card",[n("pre",[t._v('\n        nsp assemble -name <NAME> -seq <SEQ> -ss "<SS>"\n      ')])])],1)},staticRenderFns:[]}},function(t,e){t.exports={render:function(){var t=this,e=t.$createElement,n=t._self._c||e;return n("div",{attrs:{id:"app"}},[n("top-nav",{staticClass:"layout-nav"}),t._v(" "),n("div",{staticClass:"layout-page"},[n("router-view")],1)],1)},staticRenderFns:[]}},function(t,e){t.exports={render:function(){var t=this,e=t.$createElement,n=t._self._c||e;return n("el-row",[n("el-col",{attrs:{span:16,offset:4}},[n("el-card",{staticClass:"download"},[n("ul",[n("li",[n("a",{attrs:{href:"http://biophy.hust.edu.cn/download/nsp.tar.gz"}},[t._v("Source code")])]),t._v(" "),n("li",[n("a",{attrs:{href:"http://biophy.hust.edu.cn/download/nsp-lib.tar.gz"}},[t._v("Templates library")])])])])],1)],1)},staticRenderFns:[]}},,,function(t,e){}]);
//# sourceMappingURL=app.7e5c9bccd3a12a0968ae.js.map