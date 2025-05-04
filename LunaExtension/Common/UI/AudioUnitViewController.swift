//
//  AudioUnitViewController.swift
//  LunaExtension
//
//  Created by Thomas Power on 04/05/2025.
//

import Combine
import CoreAudioKit
import os
import SwiftUI

private let log = Logger(subsystem: "com.wildsurmisemusic.LunaExtension", category: "AudioUnitViewController")

@MainActor
public class AudioUnitViewController: AUViewController, AUAudioUnitFactory {
	var audioUnit: AUAudioUnit?

	var hostingController: HostingController<LunaExtensionMainView>?

	private var observation: NSKeyValueObservation?

	/* iOS View lifcycle
	 public override func viewWillAppear(_ animated: Bool) {
	 	super.viewWillAppear(animated)

	 	// Recreate any view related resources here..
	 }

	 public override func viewDidDisappear(_ animated: Bool) {
	 	super.viewDidDisappear(animated)

	 	// Destroy any view related content here..
	 }
	 */

	/* macOS View lifcycle
	 public override func viewWillAppear() {
	 	super.viewWillAppear()

	 	// Recreate any view related resources here..
	 }

	 public override func viewDidDisappear() {
	 	super.viewDidDisappear()

	 	// Destroy any view related content here..
	 }
	 */

	deinit {}

	override public func viewDidLoad() {
		super.viewDidLoad()

		// Accessing the `audioUnit` parameter prompts the AU to be created via createAudioUnit(with:)
		guard let audioUnit = audioUnit else {
			return
		}
		configureSwiftUIView(audioUnit: audioUnit)
	}

	public nonisolated func createAudioUnit(with componentDescription: AudioComponentDescription) throws -> AUAudioUnit {
		return try DispatchQueue.main.sync {
			audioUnit = try LunaExtensionAudioUnit(componentDescription: componentDescription, options: [])

			guard let audioUnit = self.audioUnit as? LunaExtensionAudioUnit else {
				log.error("Unable to create LunaExtensionAudioUnit")
				return audioUnit!
			}

			defer {
				// Configure the SwiftUI view after creating the AU, instead of in viewDidLoad,
				// so that the parameter tree is set up before we build our @AUParameterUI properties
				DispatchQueue.main.async {
					self.configureSwiftUIView(audioUnit: audioUnit)
				}
			}

			audioUnit.setupParameterTree(LunaExtensionParameterSpecs.createAUParameterTree())

			self.observation = audioUnit.observe(\.allParameterValues, options: [.new]) { _, _ in
				guard let tree = audioUnit.parameterTree else { return }

				// This insures the Audio Unit gets initial values from the host.
				for param in tree.allParameters {
					param.value = param.value
				}
			}

			guard audioUnit.parameterTree != nil else {
				log.error("Unable to access AU ParameterTree")
				return audioUnit
			}

			return audioUnit
		}
	}

	private func configureSwiftUIView(audioUnit: AUAudioUnit) {
		if let host = hostingController {
			host.removeFromParent()
			host.view.removeFromSuperview()
		}

		guard let observableParameterTree = audioUnit.observableParameterTree else {
			return
		}
		let content = LunaExtensionMainView(parameterTree: observableParameterTree)
		let host = HostingController(rootView: content)
		addChild(host)
		host.view.frame = view.bounds
		view.addSubview(host.view)
		hostingController = host

		// Make sure the SwiftUI view fills the full area provided by the view controller
		host.view.translatesAutoresizingMaskIntoConstraints = false
		host.view.topAnchor.constraint(equalTo: view.topAnchor).isActive = true
		host.view.leadingAnchor.constraint(equalTo: view.leadingAnchor).isActive = true
		host.view.trailingAnchor.constraint(equalTo: view.trailingAnchor).isActive = true
		host.view.bottomAnchor.constraint(equalTo: view.bottomAnchor).isActive = true
		view.bringSubviewToFront(host.view)
	}
}
