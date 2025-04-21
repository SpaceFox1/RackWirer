use pipewire::{
  context::{self, Context}, core::Core, main_loop::MainLoop, properties::properties, spa::{
    sys::{
      spa_dict, spa_dict_item, spa_node_add_port, spa_param_info, spa_pod_builder_fraction, spa_pod_fraction, spa_port_info, SPA_PARAM_Buffers, SPA_PARAM_EnumFormat, SPA_PARAM_Format, SPA_PARAM_Meta, SPA_TYPE_INTERFACE_Node, SPA_PARAM_INFO_READ, SPA_PARAM_INFO_WRITE, SPA_PARAM_IO, SPA_PORT_CHANGE_MASK_FLAGS, SPA_PORT_CHANGE_MASK_PARAMS, SPA_PORT_CHANGE_MASK_PROPS
    },
    utils::{Direction, Fraction}
  }, sys::{
    pw_context_create_link, pw_context_create_node, pw_core_export, pw_impl_port_add, pw_impl_port_get_node, pw_impl_port_get_properties, pw_port_info, pw_properties_new, pw_properties_new_dict, PW_KEY_FORMAT_DSP, PW_KEY_LINK_INPUT_PORT, PW_KEY_LINK_OUTPUT_PORT, PW_KEY_MEDIA_CATEGORY, PW_KEY_MEDIA_ROLE, PW_KEY_MEDIA_TYPE, PW_KEY_NODE_AUTOCONNECT, PW_KEY_NODE_EXCLUSIVE, PW_KEY_NODE_NAME
  }
};

use std::{ffi::{CStr, CString}, fs::read, time::Duration};

fn createNode(context: &Context, core: &Core) -> Result<(), Box<dyn std::error::Error>> {
  unsafe {
    let mut changemask = SPA_PORT_CHANGE_MASK_FLAGS;
    changemask = changemask | SPA_PORT_CHANGE_MASK_PROPS;
    changemask = changemask | SPA_PORT_CHANGE_MASK_PARAMS;
    
    let params = [
      spa_param_info{id:SPA_PARAM_EnumFormat,flags:SPA_PARAM_INFO_READ, user: 0, seq: 0, padding: [0, 0, 0, 0] },
      spa_param_info{id:SPA_PARAM_Meta,flags:SPA_PARAM_INFO_READ, user: 0, seq: 0, padding: [0, 0, 0, 0] },
      spa_param_info{id:SPA_PARAM_IO,flags:SPA_PARAM_INFO_READ, user: 0, seq: 0, padding: [0, 0, 0, 0] },
      spa_param_info{id:SPA_PARAM_Format,flags:SPA_PARAM_INFO_WRITE, user: 0, seq: 0, padding: [0, 0, 0, 0] },
      spa_param_info{id:SPA_PARAM_Buffers, flags:0, user: 0, seq: 0, padding: [0, 0, 0, 0] },
    ].as_mut_ptr();

    let propsItemsValueStringPointer = CString::new("32 bit float mono audio")?;
    let propsItems: spa_dict_item = spa_dict_item{
      key: PW_KEY_FORMAT_DSP.as_ptr() as *const i8,
      value: propsItemsValueStringPointer.as_ptr(),
    };

    let props = spa_dict {
      flags: 0,
      n_items: 1,
      items: &propsItems,
    };

    let node1OutPortParams = spa_port_info{
      change_mask: changemask as u64,
      flags: 0,
      rate: Fraction { num: 44100, denom: 128 },
      props: &props,
      params,
      n_params: 5,
    };
    let node1OutPort = pipewire::sys::pw_context_create_port(
      context.as_raw_ptr(),
      Direction::Output.as_raw(),
      1,
      &node1OutPortParams,
      0
    );

    // Create properties for node1
    let props1 = pw_properties_new(
      PW_KEY_NODE_AUTOCONNECT.as_ptr() as *const i8, CString::new("true")?.as_ptr(),
      0
    );
    pipewire::sys::pw_properties_set(props1, PW_KEY_NODE_EXCLUSIVE.as_ptr() as *const i8, CString::new("true")?.as_ptr());
    pipewire::sys::pw_properties_set(props1, PW_KEY_MEDIA_TYPE.as_ptr() as *const i8, CString::new("Audio")?.as_ptr());
    pipewire::sys::pw_properties_set(props1, PW_KEY_MEDIA_CATEGORY.as_ptr() as *const i8, CString::new("Playback")?.as_ptr());
    pipewire::sys::pw_properties_set(props1, PW_KEY_MEDIA_ROLE.as_ptr() as *const i8, CString::new("Music")?.as_ptr());
    pipewire::sys::pw_properties_set(props1, PW_KEY_NODE_NAME.as_ptr() as *const i8, CString::new("node1")?.as_ptr());

    // Create node1
    let node1 = pw_context_create_node(
      context.as_raw_ptr(), 
      props1, 
      0
    );

    if node1.is_null() {
        eprintln!("Failed to create node1");
        return Ok(());
    }

    let portAddResult = pw_impl_port_add(node1OutPort, node1);
    println!("Result: {}", portAddResult);

    let nodeOfPort = pw_impl_port_get_node(node1OutPort);
    if (nodeOfPort.is_null()) {
      println!("Port has not been added to a node!");

      let portPorps = pw_impl_port_get_properties(node1OutPort);
      let readData = portPorps.read();
      for i in  0..readData.dict.n_items {
        let item = *readData.dict.items.offset(i.try_into().unwrap());
        println!("Read Item index: {}, key {} - Value {}", i, CStr::from_ptr(item.key).to_str()?, CStr::from_ptr(item.value).to_str()?);
      }

    } else {
      println!("Port has been added to node");
    }

    // // Create properties for node2
    // let props2 = pw_properties_new(ptr::null(), ptr::null());
    // pipewire::sys::pw_properties_set(props2, PW_KEY_NODE_NAME.as_ptr() as *const i8, CString::new("node2")?.as_ptr());

    // // Create node2
    // let node2 = pw_context_create_node(context.as_raw_ptr(), props2, 0);
    // if node2.is_null() {
    //     eprintln!("Failed to create node2");
    //     return Ok(());
    // }

    // // Create properties for the link
    // let link_props = pw_properties_new(ptr::null(), ptr::null());
    // pipewire::sys::pw_properties_set(link_props, PW_KEY_LINK_OUTPUT_PORT.as_ptr() as *const i8, CString::new("0")?.as_ptr());
    // pipewire::sys::pw_properties_set(link_props, PW_KEY_LINK_INPUT_PORT.as_ptr() as *const i8, CString::new("0")?.as_ptr());

    // // Create link between node1 and node2
    // let link = pw_context_create_link(
    //     context.as_raw_ptr(),
    //     node1,
    //     0, // output port
    //     node2,
    //     0, // input port
    //     ptr::null_mut(), // format_filter
    //     link_props,
    //     0, // user_data_size
    // );

    // if link.is_null() {
    //     eprintln!("Failed to create link between nodes");
    // } else {
    //     println!("Link successfully created between node1 and node2");
    // }

    let trueText = CString::new("true")?;
    let audioText = CString::new("Audio")?;
    let playbackText = CString::new("Playback")?;
    let musicText = CString::new("Music")?;

    let propsExp = spa_dict {
      flags: 0,
      n_items: 5,
      items: [
        spa_dict_item{
          key: PW_KEY_NODE_AUTOCONNECT.as_ptr() as *const i8,
          value: trueText.as_ptr(),
        },
        spa_dict_item{
          key: PW_KEY_NODE_EXCLUSIVE.as_ptr() as *const i8,
          value: trueText.as_ptr(),
        },
        spa_dict_item{
          key: PW_KEY_MEDIA_TYPE.as_ptr() as *const i8,
          value: audioText.as_ptr(),
        },
        spa_dict_item{
          key: PW_KEY_MEDIA_CATEGORY.as_ptr() as *const i8,
          value: playbackText.as_ptr(),
        },
        spa_dict_item{
          key: PW_KEY_MEDIA_ROLE.as_ptr() as *const i8,
          value: musicText.as_ptr(),
        }
      ].as_ptr(),
    };

    let exported = pw_core_export(
      core.as_raw_ptr(),
      SPA_TYPE_INTERFACE_Node.as_ptr() as *const i8,
      &propsExp,
      node1 as *const _ as *mut _,
      0,
    );

    if exported.is_null() {
      eprintln!("Failed to export the node");
    } else {
      println!("Node exported to PipeWire core");
    }

    Ok(())
  }
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
  pipewire::init();
  // Initialize and start the main loop
  let mainloop = MainLoop::new(None)?;
  let context = Context::new(&mainloop)?;
  let core = context.connect(None)?;
  let registry = core.get_registry()?;

  let timer = mainloop.loop_().add_timer(move |_| {
    let _ = createNode(&context, &core);
  });

  timer.update_timer(Some(Duration::from_secs(5)), None);

  mainloop.run();

  Ok(())
}
