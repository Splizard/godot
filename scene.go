// Package godot provides methods for working with Godot data.
package godot

import (
	"fmt"
	"os"
	"path/filepath"
	"strings"
)

type Project struct {
	Path string
}

func Open(path string) (*Project, error) {
	_, err := os.Open(filepath.Join(path, "project.godot"))
	if err != nil {
		return nil, err
	}
	return &Project{Path: path}, nil
}

// NodeType identifies the type of a node.
type NodeType string

// Node represents a Godot scene node.
type Node struct {
	Name     string
	Type     NodeType
	Instance *Resource
	Parent   *Node
	Children []Node
}

// ResourceType identifies the type of a resource.
type ResourceType string

const (
	PackedScene ResourceType = "PackedScene"
)

// Resource represents a Godot resource.
type Resource struct {
	Path string
	Type ResourceType
}

// Scene represents a Godot scene.
type Scene struct {
	Nodes []Node
}

// MarshalText returns the scene in escn/tscn format.
func (s Scene) MarshalText() (string, error) {
	var escn strings.Builder

	// extract resources from the scene.
	var resources map[*Resource]int
	for _, node := range s.Nodes {
		if _, ok := resources[node.Instance]; !ok {
			resources[node.Instance] = len(resources)
		}
	}

	fmt.Fprintf(&escn, "[gd_scene load_steps=%d format=2]\n", len(resources)+1)

	for resource, i := range resources {
		if resource.Path == "" {
			return "", fmt.Errorf("resource path is empty, only external resources are supported")
		}
		fmt.Fprintf(&escn, "[ext_resource path=%q type=%q id=%d]", resource.Path, resource.Type, i)
	}

	return escn.String(), nil
}
